// CorderChannelSwap.C
// Copyright (c) 2009 The Foundry Visionmongers Ltd.  All Rights Reserved.

static const char* const HELP = "Channel swaps Red->Green, Green->Blue, Blue->Red";

#include "DDImage/PixelIop.h"
#include "DDImage/Row.h"
#include "DDImage/Knobs.h"

using namespace DD::Image;

class CorderChannelSwap : public PixelIop
{
  float value[4];
public:
  void in_channels(int input, ChannelSet& mask) const;
  CorderChannelSwap(Node* node) : PixelIop(node)
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

void CorderChannelSwap::_validate(bool for_real)
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

void CorderChannelSwap::in_channels(int input, ChannelSet& mask) const
{
  // mask is unchanged
}

void CorderChannelSwap::pixel_engine(const Row& in, int y, int x, int r,
                       ChannelMask channels, Row& out)
{
  foreach (z, channels) {
    Channel swap;	
    if (z == Chan_Red) {
	swap = Chan_Green;
    }else if (z == Chan_Green) { 
	swap = Chan_Blue;
    }else if (z == Chan_Blue) {
	swap = Chan_Red;
    }
    const float* inptr = in[swap] + x;
    const float* END = inptr + (r - x);
    float* outptr = out.writable(z) + x;
    while (inptr < END)
      *outptr++ = *inptr++;
  }
}

void CorderChannelSwap::knobs(Knob_Callback f)
{
 AColor_knob(f, value, IRange(0,4), "value");
}

#include "DDImage/NukeWrapper.h"

static Iop* build(Node* node) { return new NukeWrapper(new CorderChannelSwap(node)); }
const Iop::Description CorderChannelSwap::d("CorderChannelSwap", "Color/Math/CorderChannelSwap", build);
