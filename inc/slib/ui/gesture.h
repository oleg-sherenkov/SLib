#ifndef CHECKHEADER_SLIB_UI_GESTURE
#define CHECKHEADER_SLIB_UI_GESTURE

#include "definition.h"

#include "motion_tracker.h"

namespace slib
{

	enum class GestureType
	{
		SwipeLeft = 0,
		SwipeRight = 1,
		SwipeUp = 2,
		SwipeDown = 3,
		
		Count = 4
	};
	
	class GestureEvent : public Referable
	{
	public:
		GestureType type;
		
	public:
		GestureEvent();
		
		~GestureEvent();
		
	public:
		Ref<GestureEvent> duplicate();
		
	};
	
	class View;
	class UIEvent;
	
	class GestureRecognizer;
	
	class GestureDetector : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		GestureDetector(const Ref<View>& view);
		
		~GestureDetector();

	public:
		void enable(GestureType type);
		
		void enableNative();
		
		void processEvent(UIEvent* ev);
		
	private:
		static sl_bool _enableNative(const Ref<View>& view, GestureType type);
		
	protected:
		WeakRef<View> m_view;
		MotionTracker m_tracker;
		Ref<GestureRecognizer> m_recognizers[(int)(GestureType::Count)];
		
		friend class GestureRecognizer;
	};

}

#endif
