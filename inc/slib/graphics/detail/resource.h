#ifndef CHECKHEADER_SLIB_GRAPHICS_DETAIL_RESOURCE
#define CHECKHEADER_SLIB_GRAPHICS_DETAIL_RESOURCE

#include "../resource.h"

namespace slib
{
	
	class _ImageResourceEntry
	{
	public:
		sl_bool flagValid;

		sl_uint32 width;
		sl_uint32 height;

		const sl_uint8* source_bytes;
		sl_uint32 source_size;

		sl_int32 lock;
		void* image;
		sl_bool flag_load;

	public:
		Ref<Image> getImage();

		Ref<Image> getMatchingImage(sl_uint32 width, sl_uint32 height);

	};
	
	class _ImageResourceFreeStatic
	{
	public:
		_ImageResourceFreeStatic(_ImageResourceEntry* entries);

		~_ImageResourceFreeStatic();

	private:
		_ImageResourceEntry* m_entries;

	};
	
	Ref<Image> _ImageResource_getImage(_ImageResourceEntry* entries, sl_uint32 requiredWidth, sl_uint32 requiredHeight);
	
	List< Ref<Image> > _ImageResource_getImages(_ImageResourceEntry* entries);
	
	Ref<Drawable> _ImageResource_get(_ImageResourceEntry* entries, sl_uint32 width, sl_uint32 height);

}

#endif
