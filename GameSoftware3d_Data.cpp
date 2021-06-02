#include "GameSoftware3d_Data.h"


RasterMode& operator++ (RasterMode& rmode, int)
{
	rmode = static_cast<RasterMode>((int)rmode + 1);
	if (rmode == RasterMode::None) rmode = static_cast<RasterMode>(0);
	return rmode;
}
std::ostream& operator<< (std::ostream& stm, RasterMode rmode)
{
	switch (rmode)
	{
	case RasterMode::WireFrame: return stm << "WireFrame";
	case RasterMode::Filled: return stm << "Filled";
	case RasterMode::WireFrameFilled: return stm << "WireFrame Filled";
	case RasterMode::AffineTextureMapped: return stm << "Affine Texture Mapped";
	case RasterMode::WireFrameAffTexture: return stm << "WireFrame Affine Texture Mapped";
	case RasterMode::ProjectionTextureMapped: return stm << "Projection Correct Texture Mapped";
	case RasterMode::WireFrameProjTexture: return stm << "WireFrame Projection Correct Texture Mapped";
	default: return stm << "Unknown Enumerator";
	}
}