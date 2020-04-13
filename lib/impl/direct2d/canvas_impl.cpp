/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <canvas_impl.hpp>

namespace cycfi::artist
{
   namespace detail
   {
      struct factory_maker
      {
         factory_maker()
         {
            D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &ptr);
         }

         ~factory_maker()
         {
            release(ptr);
         }

         d2d_factory* ptr = nullptr;
      };
   }

   d2d_factory& get_factory()
   {
      static detail::factory_maker maker;
      return *maker.ptr;
   }

   void canvas_impl::update()
   {
      if (!_d2d_canvas)
      {
         RECT rc;
         GetClientRect(_hwnd, &rc);

         D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
         );

         // Create a Direct2D render target.
         auto hr = get_factory().CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(_hwnd, size),
            &_d2d_canvas
         );

         if (!SUCCEEDED(hr))
            throw std::runtime_error{ "Error: Failed to create RenderTarget." };
      }

      if (_state)
         _state->update(*_d2d_canvas);
   }
}
