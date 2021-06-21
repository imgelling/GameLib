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

RasterType& operator++ (RasterType& rType, int)
{
	rType = static_cast<RasterType>(static_cast<int>(rType) + 1);
	if (rType == RasterType::None) rType = static_cast<RasterType>(0);
	return rType;
}
std::ostream& operator<< (std::ostream& stm, RasterType rmode)
{
	switch (rmode)
	{
	case RasterType::Barycentric: return stm << "Barycentric (Slower but smoother)"; 
	case RasterType::ScanLine: return stm << "Scan Line (Faster but noisey)";
	default: return stm << "Unknown Enumerator";
	}
}