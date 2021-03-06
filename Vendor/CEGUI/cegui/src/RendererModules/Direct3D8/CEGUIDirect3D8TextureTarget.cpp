/***********************************************************************
    filename:   CEGUIDirect3D8TextureTarget.cpp
    created:    Thu Jul 29 2010
    author:     Mark Rohrbacher
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#define NOMINMAX
#include "CEGUIDirect3D8TextureTarget.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIRenderQueue.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIDirect3D8Renderer.h"
#include "CEGUIDirect3D8Texture.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float Direct3D8TextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
Direct3D8TextureTarget::Direct3D8TextureTarget(Direct3D8Renderer& owner) :
    Direct3D8RenderTarget(owner),
    d_texture(0),
    d_surface(0)
{
    // this essentially creates a 'null' CEGUI::Texture
    d_CEGUITexture = &static_cast<Direct3D8Texture&>(d_owner.createTexture(0));

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Size(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
Direct3D8TextureTarget::~Direct3D8TextureTarget()
{
    cleanupRenderTexture();
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::declareRenderSize(const Size& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    setArea(Rect(d_area.getPosition(), sz));
    resizeRenderTexture();
    clear();
}

//----------------------------------------------------------------------------//
bool Direct3D8TextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::activate()
{
    enableRenderTexture();
    Direct3D8RenderTarget::activate();
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::deactivate()
{
    Direct3D8RenderTarget::deactivate();
    disableRenderTexture();
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::clear()
{
    // switch to targetting our texture
    enableRenderTexture();
    // Clear it.
    d_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    // switch back to rendering to previous target
    disableRenderTexture();
}

//----------------------------------------------------------------------------//
Texture& Direct3D8TextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::initialiseRenderTexture()
{
    Size tex_sz(d_owner.getAdjustedSize(d_area.getSize()));

    d_device->CreateTexture(static_cast<UINT>(tex_sz.d_width),
                            static_cast<UINT>(tex_sz.d_height),
                            1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
                            D3DPOOL_DEFAULT, &d_texture);

    d_texture->GetSurfaceLevel(0, &d_surface);

    // wrap the created texture with the CEGUI::Texture
    d_CEGUITexture->setDirect3D8Texture(d_texture);
    d_CEGUITexture->setOriginalDataSize(d_area.getSize());
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::resizeRenderTexture()
{
    cleanupRenderTexture();
    initialiseRenderTexture();
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::cleanupRenderTexture()
{
    if (d_surface)
    {
        d_surface->Release();
        d_surface = 0;
    }
    if (d_texture)
    {
        d_CEGUITexture->setDirect3D8Texture(0);
        d_texture->Release();
        d_texture = 0;
    }
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::enableRenderTexture()
{
    LPDIRECT3DSURFACE8 oldSurface;
    LPDIRECT3DSURFACE8 oldDepthStencilSurface;
    d_device->GetRenderTarget(&oldSurface);
    d_device->GetDepthStencilSurface(&oldDepthStencilSurface);

    if (oldSurface && oldSurface != d_surface)
    {
        d_prevColourSurface = oldSurface;
        d_prevDepthStencilSurface = oldDepthStencilSurface;
        d_device->SetRenderTarget(d_surface,0);
    }
    else
    {
        if (oldSurface)
            oldSurface->Release();
        if (oldDepthStencilSurface)
            oldDepthStencilSurface->Release();
    }
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::disableRenderTexture()
{
    d_device->SetRenderTarget(d_prevColourSurface, d_prevDepthStencilSurface);
    d_prevColourSurface->Release();
    d_prevDepthStencilSurface->Release();
}

//----------------------------------------------------------------------------//
bool Direct3D8TextureTarget::isRenderingInverted() const
{
    return false;
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::preD3DReset()
{
    if (d_surface)
    {
        d_surface->Release();
        d_surface = 0;
    }

    if (d_CEGUITexture)
        d_CEGUITexture->preD3DReset();

    if (d_texture)
    {
        d_texture->Release();
        d_texture = 0;
    }
}

//----------------------------------------------------------------------------//
void Direct3D8TextureTarget::postD3DReset()
{
    if (!d_CEGUITexture)
        return;

    // this will recreate the texture
    d_CEGUITexture->postD3DReset();
    // we now obtain a reference to that created texture
    d_texture = d_CEGUITexture->getDirect3D8Texture();
    if (d_texture)
    {
        d_texture->AddRef();
        // now obtain the surface
        d_texture->GetSurfaceLevel(0, &d_surface);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
