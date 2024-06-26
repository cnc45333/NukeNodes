// CorderGamma.C
// Copyright (c) 2009 The Foundry Visionmongers Ltd.  All Rights Reserved.

static const char* const HELP = "Multiplies by gamma for a set of channels";

#include "DDImage/PixelIop.h"
#include "DDImage/Row.h"
#include "DDImage/Knobs.h"

using namespace DD::Image;

class CorderGamma : public PixelIop
{
  float value[4];
public:
  void in_channels(int input, ChannelSet& mask) const;
  CorderGamma(Node* node) : PixelIop(node)
  {
    value[0] = value[1] = value[2] = value[3] = 0;
  }
  bool pass_transform() const { return true; }
  void pixel_engine(const Row &in, int y, int x, int r, ChannelMask, Row & out);
  virtual void knobs(Knob_Callback);
  static const Iop::Description d;
  const char* Class() const { return d.name; }
  const char* node_help() const { return HELP; }
  void _validate(bool);
};

void CorderGamma::_validate(bool for_real)
{
  copy_info();
  for (unsigned i = 0; i < 4; i++) {
    if (value[i]) {
      set_out_channels(Mask_All);
      info_.black_outside(false);
      return;
    }
  }
  set_out_channels(Mask_None);
}

void CorderGamma::in_channels(int input, ChannelSet& mask) const
{
  // mask is unchanged
}

void CorderGamma::pixel_engine(const Row& in, int y, int x, int r,
                       ChannelMask channels, Row& out)
{
  foreach (z, channels) {
    const float gamma = value[colourIndex(z)];
    const float* inptr = in[z] + x;
    const float* END = inptr + (r - x);
    float* outptr = out.writable(z) + x;
    while (inptr < END)
      *outptr++ = pow(*inptr++, (1/gamma)) ;
  }
}

void CorderGamma::knobs(Knob_Callback f)
{
  AColor_knob(f, value, IRange(0, 4), "Gamma Value");
}

#include "DDImage/NukeWrapper.h"

static Iop* build(Node* node) { return new NukeWrapper(new CorderGamma(node)); }
const Iop::Description CorderGamma::d("CorderGamma", "Color/Math/CorderGamma", build);
