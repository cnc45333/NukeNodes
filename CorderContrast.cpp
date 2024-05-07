// CoderContrast.C
// Copyright (c) 2009 The Foundry Visionmongers Ltd.  All Rights Reserved.

static const char* const HELP = "Increases contrast for a set of channels";

#include "DDImage/PixelIop.h"
#include "DDImage/Row.h"
#include "DDImage/Knobs.h"

using namespace DD::Image;

class CorderContrast : public PixelIop
{
  float value[4];
public:
  void in_channels(int input, ChannelSet& mask) const;
  CorderContrast(Node* node) : PixelIop(node)
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

void CorderContrast::_validate(bool for_real)
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

void CorderContrast::in_channels(int input, ChannelSet& mask) const
{
  // mask is unchanged
}

void CorderContrast::pixel_engine(const Row& in, int y, int x, int r,
                       ChannelMask channels, Row& out)
{
  //Output = (Input/0.18)^Contrast*0.18
  foreach (z, channels) {
    const float contrast = value[colourIndex(z)];
    const float* inptr = in[z] + x;
    const float* END = inptr + (r - x);
    float* outptr = out.writable(z) + x;
    while (inptr < END)
      *outptr++ = pow((*inptr++/0.18), contrast) * 0.18;
  }
}

void CorderContrast::knobs(Knob_Callback f)
{
  AColor_knob(f, value, IRange(0, 4), "contrast");

}

#include "DDImage/NukeWrapper.h"

static Iop* build(Node* node) { return new NukeWrapper(new CorderContrast(node)); }
const Iop::Description CorderContrast::d("CorderContrast", "Color/Math/CorderContrast", build);
