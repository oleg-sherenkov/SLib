/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/render_view.h"
#include "slib/render/opengl.h"

#include "view_win32.h"

namespace slib
{

	class Win32_RenderViewInstance : public Win32_ViewInstance
	{
	public:
		AtomicRef<Renderer> m_renderer;

	public:
		Win32_RenderViewInstance()
		{
		}

		~Win32_RenderViewInstance()
		{
			Ref<Renderer> renderer = m_renderer;
			if (renderer.isNotNull()) {
				renderer->release();
			}
		}

	public:
		void setRenderer(const Ref<Renderer>& renderer, RedrawMode redrawMode)
		{
			m_renderer = renderer;
			if (renderer.isNotNull()) {
				renderer->setRenderingContinuously(redrawMode == RedrawMode::Continuously);
			}
		}

		sl_bool preprocessWindowMessage(MSG& msg) override
		{
			return sl_false;
		}

		sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result) override
		{
			if (msg == WM_PAINT) {
				PAINTSTRUCT ps;
				::BeginPaint(m_handle, &ps);
				::EndPaint(m_handle, &ps);
				Ref<Renderer> renderer = m_renderer;
				if (renderer.isNotNull()) {
					renderer->requestRender();
				}
				return sl_true;
			} else if (msg == WM_ERASEBKGND) {
				result = TRUE;
				return sl_true;
			}
			return Win32_ViewInstance::processWindowMessage(msg, wParam, lParam, result);
		}

		void onFrame(RenderEngine* engine)
		{
			Ref<View> _view = getView();
			if (RenderView* view = CastInstance<RenderView>(_view.get())) {
				view->dispatchFrame(engine);
			}
		}
	};

	Ref<ViewInstance> RenderView::createNativeWidget(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		DWORD styleEx = 0;
		DWORD style = 0;
		Ref<Win32_RenderViewInstance> ret = Win32_ViewInstance::create<Win32_RenderViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
		if (ret.isNotNull()) {
			RenderEngineType engineType = getPreferredEngineType();
			if (engineType == RenderEngineType::OpenGL_ES) {
				EGL::loadEntries();
				GLES::loadEntries();
				if (!(EGL::isAvailable() && GLES::isAvailable())) {
					engineType = RenderEngineType::OpenGL;
				}
			} else if (engineType == RenderEngineType::OpenGL) {
			} else {
				engineType = RenderEngineType::OpenGL;
			}
			if (engineType == RenderEngineType::OpenGL_ES) {
				RendererParam rp;
				rp.onFrame = SLIB_FUNCTION_WEAKREF(Win32_RenderViewInstance, onFrame, ret);
				Ref<Renderer> renderer = EGL::createRenderer((void*)(ret->getHandle()), rp);
				if (renderer.isNotNull()) {
					ret->setRenderer(renderer, m_redrawMode);
					return ret;
				}
			} else if (engineType == RenderEngineType::OpenGL) {
				RendererParam rp;
				rp.onFrame = SLIB_FUNCTION_WEAKREF(Win32_RenderViewInstance, onFrame, ret);
				Ref<Renderer> renderer = WGL::createRenderer((void*)(ret->getHandle()), rp);
				if (renderer.isNotNull()) {
					ret->setRenderer(renderer, m_redrawMode);
					return ret;
				}
			}
		}
		return sl_null;
	}

	void RenderView::_setRedrawMode_NW(RedrawMode mode)
	{
		Ref<Win32_RenderViewInstance> instance = Ref<Win32_RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			Ref<Renderer> renderer = instance->m_renderer;
			if (renderer.isNotNull()) {
				renderer->setRenderingContinuously(mode == RedrawMode::Continuously);
			}
		}
	}

	void RenderView::_requestRender_NW()
	{
		Ref<Win32_RenderViewInstance> instance = Ref<Win32_RenderViewInstance>::from(getViewInstance());
		if (instance.isNotNull()) {
			Ref<Renderer> renderer = instance->m_renderer;
			if (renderer.isNotNull()) {
				renderer->requestRender();
			}
		}
	}

}

#endif
