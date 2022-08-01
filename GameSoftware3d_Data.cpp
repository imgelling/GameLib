#include "GameSoftware3d_Data.h"


RasterMode& operator++ (RasterMode& rmode, int)
{
	rmode = static_cast<RasterMode>((int)rmode + 1);
	if (rmode == RasterMode::None) rmode = static_cast<RasterMode>(0);
	return rmode;
}
std::ostream& operator<< (std::ostream& stream, RasterMode rmode)
{
	switch (rmode)
	{
	case RasterMode::WireFrame: return stream << "WireFrame";
	case RasterMode::Filled: return stream << "Filled";
	case RasterMode::WireFrameFilled: return stream << "WireFrame Filled";
	case RasterMode::AffineTextureMapped: return stream << "Affine Texture Mapped";
	case RasterMode::WireFrameAffTexture: return stream << "WireFrame Affine Texture Mapped";
	case RasterMode::ProjectionTextureMapped: return stream << "Projection Correct Texture Mapped";
	case RasterMode::WireFrameProjTexture: return stream << "WireFrame Projection Correct Texture Mapped";
	default: return stream << "Unknown Enumerator";
	}
}

RasterType& operator++ (RasterType& rType, int)
{
	rType = static_cast<RasterType>(static_cast<int>(rType) + 1);
	if (rType == RasterType::None) rType = static_cast<RasterType>(0);
	return rType;
}
std::ostream& operator<< (std::ostream& stream, RasterType rmode)
{
	switch (rmode)
	{
	case RasterType::Barycentric: return stream << "Barycentric (Slower but smoother)"; 
	case RasterType::ScanLine: return stream << "Scan Line (Faster but noisey)";
	default: return stream << "Unknown Enumerator";
	}
}