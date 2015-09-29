// disable Photon: touch /etc/system/config/nophoton
// start QNX Advanced Graphics: io-display -dvid=0x15ad,did=0x405 -> see display.conf in /etc/system/config for details

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <gf/gf.h>
#include <img/img.h>
#include "The Simpsons - Image (RGB565).h"
#include "The Simpsons - Image (RGBA8888).h"
#include "The Simpsons - Image (Alpha Channel).h"
#include "The Simpsons - Text (Alpha Map).h"

int drawWhiteBackground(gf_display_info_t* displayInfo, gf_context_t context);
int drawImageBackground(gf_display_info_t* displayInfo, gf_context_t context, int width, int height);
int pngDrawTheSimpsonsText(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context);
int alphaMapDrawTheSimpsonsText(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context);
int pngDrawTheSimpsonsImage(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context, unsigned char alphaValue);
int rgba8888DrawTheSimpsonsImage(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context, unsigned char alphaValue);
int rgb565DrawTheSimpsonsImage(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context, unsigned char alphaValue);

int main()
{
	gf_dev_t          gfDevice;
	gf_dev_info_t     gfDeviceInfo;
	gf_display_t      display;
	gf_display_info_t displayInfo;
	
	if (gf_dev_attach(&gfDevice, GF_DEVICE_INDEX(0), &gfDeviceInfo) != GF_ERR_OK)
	{
	    printf("Error: gf_dev_attach()\n");
	    return -1;
	}
	printf("Number of Displays: %d\n", gfDeviceInfo.ndisplays);	
	unsigned int i;
	for (i = 0; i < gfDeviceInfo.ndisplays; i++)
	{
	    printf("Display %d: ", i);
	    if (gf_display_attach(&display, gfDevice, i, &displayInfo) == GF_ERR_OK)
	    {
	        printf("%d x %d, Refresh Rate = %dHz\n", displayInfo.xres, displayInfo.yres, displayInfo.refresh);
	        printf("Number of Layers: %d\n", displayInfo.nlayers);
	        printf("Primary Display Layer: %d\n", displayInfo.main_layer_index);
	    }
	    else 
	    {
	        printf("Error: gf_display_attach()\n");
	    }
	}
	if(display == NULL)
	{
		printf("Error: gf_layer_attach()\n");
		return -1;
	}
	
	gf_layer_t layer;
	unsigned int layerIndex = displayInfo.main_layer_index;
    if (GF_ERR_OK != gf_layer_attach(&layer, display, layerIndex, 0))
    {
    	printf("Error: gf_layer_attach\n");
    	return -1;
    }
    printf("Display Format: %d -> ", displayInfo.format);
    switch(displayInfo.format)
    {
    	case GF_FORMAT_PACK_ARGB1555:
    	case GF_FORMAT_PKLE_ARGB1555:
    	case GF_FORMAT_PKBE_ARGB1555:
    		printf("ARGB1555");
    		break;
    	case GF_FORMAT_PACK_RGB565:
    	case GF_FORMAT_PKLE_RGB565:
    	case GF_FORMAT_PKBE_RGB565:
    		printf("RGB565");
    		break;
    	case GF_FORMAT_BGR888:
    		printf("BGR888");
    		break;
    	case GF_FORMAT_BGRA8888:
    		printf("BGRA8888");
    		break;
    	default:
    		printf("NONE");
    }
    printf("\n");
    gf_surface_t layerSurface;	    	
    int result = gf_surface_create_layer(&layerSurface, &layer, 1, 0, displayInfo.xres, displayInfo.yres, displayInfo.format, NULL, 0);
	if (GF_ERR_OK != result)
	{
		printf("Error: gf_surface_create_layer(), ID: ");
        switch(result)
		{
			case GF_ERR_MEM:
				printf("GF_ERR_MEM\n");
				break;
			case GF_ERR_PARM:
				printf("GF_ERR_PARM\n");
				break;
			case GF_ERR_IODISPLAY:
				printf("GF_ERR_IODISPLAY\n");
				break;
			default:
				printf("NONE\n");
				break;
		}
		return -1;
	}  			
	gf_layer_set_surfaces(layer, &layerSurface, 1);
	if(GF_ERR_OK != gf_layer_update(layer, 0))
	{
		printf("Error: gf_layer_update\n");
		return -1;
	}	   			
	gf_context_t context;
	if (gf_context_create(&context) != GF_ERR_OK)
	{
        printf("Error: gf_context_create\n");
        return -1;
    }
	if (gf_context_set_surface(context, layerSurface) != GF_ERR_OK) 
	{
    	printf("Error: gf_context_set_surface\n");
        return -1;
    }
    printf("QNX GF is READY!\n");
		
	drawWhiteBackground(&displayInfo, context);			
//	pngDrawTheSimpsonsImage(&gfDevice, &displayInfo, layerSurface, context, 0xC0);
//	rgba8888DrawTheSimpsonsImage(&gfDevice, &displayInfo, layerSurface, context, 0xC0);
	rgb565DrawTheSimpsonsImage(&gfDevice, &displayInfo, layerSurface, context, 0xFF);
//	pngDrawTheSimpsonsText(&gfDevice, &displayInfo, layerSurface, context);
	alphaMapDrawTheSimpsonsText(&gfDevice, &displayInfo, layerSurface, context);
	
    printf("Press <Enter> to continue ...\n");
	getchar();
	gf_context_free(context);
	gf_surface_free(layerSurface);		    
    gf_dev_detach(gfDevice);
	return 0;
}

int drawWhiteBackground(gf_display_info_t* displayInfo, gf_context_t context)
{	
	int result = gf_draw_begin( context );
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_begin\n");
    	return -1;
    }
	gf_context_set_fgcolor(context, 0xFFFFFF);
	gf_draw_rect(context, 0, 0, displayInfo->xres, displayInfo->yres);	
    gf_draw_end( context );
    return 0;
}

int drawImageBackground(gf_display_info_t* displayInfo, gf_context_t context, int width, int height)
{
	int result = gf_draw_begin( context );
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_begin\n");
    	return -1;
    }
    int x = (displayInfo->xres - width) / 2;
    int y = (displayInfo->yres - height) / 2;
    gf_context_set_fgcolor(context, 0x80FF80);
	gf_draw_rect(context, x, y, x + width - 1, y + height - 1);
    gf_draw_end(context);	
	return 0;
}

int pngDrawTheSimpsonsText(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context)
{
	img_t img;
	img_lib_t imgLib = NULL;
	int result;
	
	if((result = img_lib_attach(&imgLib)) != IMG_ERR_OK) 
	{
    	printf("Error: img_lib_attach()\n");
    	return -1;
    }
	img.flags = 0;
	if((result = img_load_file(imgLib, "/root/The Simpsons - Text.png", NULL, &img)) != IMG_ERR_OK) 
	{
        printf("Error: img_load_file\n");
        return -1;
	}
	gf_surface_t textSurface;
	result = gf_surface_create(&textSurface, *gfDevice, img.w, img.h, GF_FORMAT_BYTE, NULL, GF_SURFACE_CREATE_ALPHA_MAP);
	if(GF_ERR_OK != result)
	{
		switch(result)
		{
			case GF_ERR_MEM:
				printf("Error: gf_surface_create (GF_ERR_MEM)\n");
				break;
			case GF_ERR_PARM:
				printf("Error: gf_surface_create (GF_ERR_PARM)\n");
				break;
			case GF_ERR_IODISPLAY:
				printf("Error: gf_surface_create (GF_ERR_IODISPLAY)\n");
				break;
			default:
				printf("Error: gf_surface_create\n");
		}
		return -1;
	}
	gf_surface_info_t textSurfaceInfo;
    gf_surface_get_info(textSurface, &textSurfaceInfo);
    unsigned char* textSurfaceData = (unsigned char*) textSurfaceInfo.vaddr;
    unsigned char* imgAlphaMap = img.access.direct.data;	
	for(unsigned int i = 0; i < img.h; i++)
	{
		for(unsigned int j = 0; j < img.w; j += 1)
    	{
			textSurfaceData[j] = 0xFF - imgAlphaMap[j];			
    	}    	
    	textSurfaceData += textSurfaceInfo.stride;
    	imgAlphaMap += img.access.direct.stride;
	}
	gf_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.mode = GF_ALPHA_M1_GLOBAL | GF_ALPHA_M1_MAP | GF_BLEND_SRC_M1 | GF_BLEND_DST_1mM1;
    alpha.map = textSurface;
    alpha.m1 = 0xD0;
    gf_context_set_alpha(context, &alpha);    
    result = gf_draw_begin(context);
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_begin\n");
    	return -1;
    }
    int x = (displayInfo->xres - img.w) / 2;
    int y = (displayInfo->yres - img.h) / 2 - 140;
    gf_context_set_fgcolor(context, 0xFF8000);    
    gf_draw_rect(context, x, y, x + img.w - 1, y + img.h - 1);
    result = gf_draw_flush( context );
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_flush\n");
    	return -1;
    }
    gf_draw_end(context);
    gf_context_disable_alpha(context);
    gf_surface_free(textSurface);
    free(img.access.direct.data);
	img_lib_detach(imgLib);
    return 0;
}

int alphaMapDrawTheSimpsonsText(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context)
{
	const unsigned int textWidth = 250;
	const unsigned int textHeight = 44;
	gf_surface_t textSurface;
	int result = gf_surface_create(&textSurface, *gfDevice, textWidth, textHeight, GF_FORMAT_BYTE, NULL, GF_SURFACE_CREATE_ALPHA_MAP);
	if(GF_ERR_OK != result)
	{
		switch(result)
		{
			case GF_ERR_MEM:
				printf("Error: gf_surface_create (GF_ERR_MEM)\n");
				break;
			case GF_ERR_PARM:
				printf("Error: gf_surface_create (GF_ERR_PARM)\n");
				break;
			case GF_ERR_IODISPLAY:
				printf("Error: gf_surface_create (GF_ERR_IODISPLAY)\n");
				break;
			default:
				printf("Error: gf_surface_create\n");
		}
		return -1;
	}
	gf_surface_info_t textSurfaceInfo;
    gf_surface_get_info(textSurface, &textSurfaceInfo);
    unsigned char* textSurfaceData = (unsigned char*) textSurfaceInfo.vaddr;
	unsigned char* alphaMapData = &theSimpsonsTextAlphaMap[0];
	for(unsigned int i = 0; i < textHeight; i++)
	{
		for(unsigned int j = 0; j < textWidth; j += 1)
    	{
			textSurfaceData[j] = alphaMapData[j];
    	}    	
    	textSurfaceData += textSurfaceInfo.stride;
		alphaMapData += textWidth;
	}	
    gf_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.mode = GF_ALPHA_M1_GLOBAL | GF_ALPHA_M1_MAP | GF_BLEND_SRC_M1 | GF_BLEND_DST_1mM1;
    alpha.map = textSurface;
    alpha.m1 = 0xD0;
    gf_context_set_alpha(context, &alpha);    
    result = gf_draw_begin(context);
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_begin\n");
    	return -1;
    }    
    int x = (displayInfo->xres - textWidth) / 2;
    int y = (displayInfo->yres - textHeight) / 2 - 140;
    gf_context_set_fgcolor(context, 0xFF8000);    
    gf_draw_rect(context, x, y, x + textWidth - 1, y + textHeight - 1);
    result = gf_draw_flush( context );
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_flush\n");
    	return -1;
    }
    gf_draw_end(context);
    gf_context_disable_alpha(context);
    gf_surface_free(textSurface);
    return 0;
}

int pngDrawTheSimpsonsImage(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context, unsigned char alphaValue)
{	
	int result;
	img_t img;
	img_lib_t imgLib = NULL;
	
	if((result = img_lib_attach(&imgLib)) != IMG_ERR_OK) 
	{
    	printf("Error: img_lib_attach()\n");
    	return -1;
    }
	img.flags = 0;
//	img.format = IMG_FMT_PKLE_ARGB8888;
//	img.flags |= IMG_FORMAT;
	if((result = img_load_file(imgLib, "/root/The Simpsons - Image.png", NULL, &img)) != IMG_ERR_OK) 
	{
        printf("Error: img_load_file\n");
        return -1;
	}
	fprintf(stdout, "Image Format: %d x %d x %d\n", img.w, img.h, IMG_FMT_BPP(img.format));
	
	/* convert PNG Image to Raw Image */
//	unsigned int* imgTheSimpsons = (unsigned int*) img.access.direct.data;
//	for(unsigned int i = 0; i < img.h; i++)
//	{
//		for(unsigned int j = 0; j < img.w; j += 1)
//    	{			
//			printf("0x%08X, ", imgTheSimpsons[j]);	
//    	}	   		
//    	printf("\n");
//    	imgTheSimpsons += (img.access.direct.stride / sizeof(unsigned int));
//	}
	
	drawImageBackground(displayInfo, context, img.w, img.h);
	
	gf_surface_t surface;
	if((result = gf_surface_attach(&surface, *gfDevice, img.w, img.h, img.access.direct.stride, displayInfo->format, NULL, img.access.direct.data, 0)) != GF_ERR_OK)
	{
	    printf("Error: gf_surface_attach\n");
	    return -1;
	}
	if (img.flags & IMG_TRANSPARENCY) 
	{
		gf_chroma_t chroma;
	    memset(&chroma, 0, sizeof chroma);
	    chroma.mode = GF_CHROMA_OP_SRC_MATCH | GF_CHROMA_OP_NO_DRAW;
	    if(img.format & IMG_FMT_PALETTE)
	    {
	    	chroma.color0 = img.palette[img.transparency.index];
	    }
	    else if(IMG_FMT_BPP(img.format) < 24)
	    {
	    	chroma.color0 = img.transparency.rgb16;
	    }
	    else
	    {
	    	chroma.color0 = img.transparency.rgb32;
	    }
		gf_context_set_chroma(context, &chroma);
	}
	if (img.format & IMG_FMT_ALPHA)
	{
        gf_alpha_t alpha;
        memset(&alpha, 0, sizeof alpha);
        alpha.mode = GF_ALPHA_M1_SRC_PIXEL_ALPHA | GF_BLEND_SRC_M1 | GF_BLEND_DST_1mM1;
        if(alphaValue != 0xFF)
	    {
		    alpha.m1 = alphaValue;
		    alpha.mode |= GF_ALPHA_M1_GLOBAL;
	    }
        gf_context_set_alpha(context, &alpha);
	}	
	result = gf_draw_begin( context );
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_begin\n");
    	return -1;
    }
    int x = (displayInfo->xres - img.w) / 2;
    int y = (displayInfo->yres - img.h) / 2;
    gf_draw_blit2(context, surface, layerSurface, 0, 0, img.w - 1, img.h - 1, x, y);
	gf_draw_end(context);
	if(img.format & IMG_FMT_ALPHA)
	{
		gf_context_disable_alpha(context);
	}
	if(img.flags & IMG_TRANSPARENCY)
	{
		gf_context_disable_chroma(context);
	}	
	gf_surface_free(surface);	
	free(img.access.direct.data);
	img_lib_detach(imgLib);
	return 0;
}

int rgba8888DrawTheSimpsonsImage(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context, unsigned char alphaValue)
{
	const unsigned int imageWidth = 250;
	const unsigned int imageHeight = 286;
	
	gf_surface_t surface;
	int result = gf_surface_create(&surface, *gfDevice, imageWidth, imageHeight, GF_FORMAT_BGRA8888, NULL, 0);
	if(GF_ERR_OK != result)
	{
		switch(result)
		{
			case GF_ERR_MEM:
				printf("Error: gf_surface_create (GF_ERR_MEM)\n");
				break;
			case GF_ERR_PARM:
				printf("Error: gf_surface_create (GF_ERR_PARM)\n");
				break;
			case GF_ERR_IODISPLAY:
				printf("Error: gf_surface_create (GF_ERR_IODISPLAY)\n");
				break;
			default:
				printf("Error: gf_surface_create\n");
		}
		return -1;
	}
	gf_surface_info_t surfaceInfo;
    gf_surface_get_info(surface, &surfaceInfo);
    unsigned int* surfaceData = (unsigned int*) surfaceInfo.vaddr;    
    unsigned int* imageData = &theSimpsonsImageRGBA8888[0];
	for(unsigned int i = 0; i < imageHeight; i++)
	{
		for(unsigned int j = 0; j < imageWidth; j += 1)
    	{			
			surfaceData[j] = imageData[j];			
    	}	   		
    	surfaceData += (surfaceInfo.stride / sizeof(unsigned int));		
		imageData += imageWidth;
	}
	
	drawImageBackground(displayInfo, context, imageWidth, imageHeight);
    
	gf_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.mode = GF_ALPHA_M1_SRC_PIXEL_ALPHA | GF_BLEND_SRC_M1 | GF_BLEND_DST_1mM1;
    if(alphaValue != 0xFF)
    {
	    alpha.m1 = alphaValue;
	    alpha.mode |= GF_ALPHA_M1_GLOBAL;
    }
    gf_context_set_alpha(context, &alpha);
	result = gf_draw_begin( context );
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_begin\n");
    	return -1;
    }
    int x = (displayInfo->xres - imageWidth) / 2;
    int y = (displayInfo->yres - imageHeight) / 2;
    gf_draw_blit2(context, surface, layerSurface, 0, 0, imageWidth - 1, imageHeight - 1, x, y);    
	gf_draw_end(context);
	gf_context_disable_alpha(context);
	gf_surface_free(surface);
	return 0;
}

int rgb565DrawTheSimpsonsImage(gf_dev_t* gfDevice, gf_display_info_t* displayInfo, gf_surface_t layerSurface, gf_context_t context, unsigned char alphaValue)
{
	const unsigned int imageWidth = 250;
	const unsigned int imageHeight = 286;
	
//	gf_surface_t surface;
//	int result = gf_surface_create(&surface, *gfDevice, imageWidth, imageHeight, GF_FORMAT_PKLE_RGB565, NULL, 0);
//	if(GF_ERR_OK != result)
//	{
//		switch(result)
//		{
//			case GF_ERR_MEM:
//				printf("Error: gf_surface_create (GF_ERR_MEM)\n");
//				break;
//			case GF_ERR_PARM:
//				printf("Error: gf_surface_create (GF_ERR_PARM)\n");
//				break;
//			case GF_ERR_IODISPLAY:
//				printf("Error: gf_surface_create (GF_ERR_IODISPLAY)\n");
//				break;
//			default:
//				printf("Error: gf_surface_create\n");
//		}
//		return -1;
//	}
//	gf_surface_info_t surfaceInfo;
//	gf_surface_get_info(surface, &surfaceInfo);
//	unsigned short* surfaceData = (unsigned short*) surfaceInfo.vaddr;
//	unsigned char* imageData = (unsigned char*)&theSimpsonsImageRGBA8888[0];
//	for(unsigned int i = 0; i < imageHeight; i++)
//	{
//		for(unsigned int j = 0; j < imageWidth; j += 1)
//    	{
//    		// convert RGBA8888 to RGB565	
//			surfaceData[j] = ((imageData[j*4+0] & 0xF8) >> 3) | ((imageData[j*4+1] & 0xFC) << 3) | ((imageData[j*4+2] & 0xF8) << 8);			
//    	}
//    	surfaceData += (surfaceInfo.stride / sizeof(unsigned short));
//		imageData += imageWidth * sizeof(unsigned int);
//	}
	
	gf_surface_t surface;
	int result = gf_surface_create(&surface, *gfDevice, imageWidth, imageHeight, GF_FORMAT_PKLE_RGB565, NULL, 0);
	if(GF_ERR_OK != result)
	{
		switch(result)
		{
			case GF_ERR_MEM:
				printf("Error: gf_surface_create (GF_ERR_MEM)\n");
				break;
			case GF_ERR_PARM:
				printf("Error: gf_surface_create (GF_ERR_PARM)\n");
				break;
			case GF_ERR_IODISPLAY:
				printf("Error: gf_surface_create (GF_ERR_IODISPLAY)\n");
				break;
			default:
				printf("Error: gf_surface_create\n");
		}
		return -1;
	}
	gf_surface_info_t surfaceInfo;
    gf_surface_get_info(surface, &surfaceInfo);
    unsigned short* surfaceData = (unsigned short*) surfaceInfo.vaddr;
    unsigned short* imageData = &theSimpsonsImageRGB565[0];
	for(unsigned int i = 0; i < imageHeight; i++)
	{
		for(unsigned int j = 0; j < imageWidth; j += 1)
    	{			
			surfaceData[j] = imageData[j];
    	}   	
    	surfaceData += (surfaceInfo.stride / sizeof(unsigned short));
		imageData += imageWidth;
	}
	
	drawImageBackground(displayInfo, context, imageWidth, imageHeight);
    
	gf_surface_t alphaMapSurface;
	result = gf_surface_create(&alphaMapSurface, *gfDevice, imageWidth, imageHeight, GF_FORMAT_BYTE, NULL, GF_SURFACE_CREATE_ALPHA_MAP);
	if(GF_ERR_OK != result)
	{
		switch(result)
		{
			case GF_ERR_MEM:
				printf("Error: gf_surface_create (GF_ERR_MEM)\n");
				break;
			case GF_ERR_PARM:
				printf("Error: gf_surface_create (GF_ERR_PARM)\n");
				break;
			case GF_ERR_IODISPLAY:
				printf("Error: gf_surface_create (GF_ERR_IODISPLAY)\n");
				break;
			default:
				printf("Error: gf_surface_create\n");
		}
		return -1;
	}
	gf_surface_info_t alphaMapSurfaceInfo;
    gf_surface_get_info(alphaMapSurface, &alphaMapSurfaceInfo);
    unsigned char* alphaMapSurfaceData = (unsigned char*) alphaMapSurfaceInfo.vaddr;
	unsigned char* alphaMapData = &theSimpsonsImageAlphaChannel[0];
	for(unsigned int i = 0; i < imageHeight; i++)
	{
		for(unsigned int j = 0; j < imageWidth; j += 1)
    	{
			alphaMapSurfaceData[j] = alphaMapData[j];
    	}    	
    	alphaMapSurfaceData += alphaMapSurfaceInfo.stride;
		alphaMapData += imageWidth;
	}
	gf_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.mode = GF_ALPHA_M1_MAP | GF_BLEND_SRC_M1 | GF_BLEND_DST_1mM1;
    alpha.map = alphaMapSurface;
    if(alphaValue != 0xFF)
    {
	    alpha.m1 = alphaValue;
	    alpha.mode |= GF_ALPHA_M1_GLOBAL;
    } 
	gf_context_set_alpha(context, &alpha);	
	result = gf_draw_begin( context );
    if(GF_ERR_OK != result)
    {
    	printf("Error: gf_draw_begin\n");
    	return -1;
    }
    int x = (displayInfo->xres - imageWidth) / 2;
    int y = (displayInfo->yres - imageHeight) / 2;
    gf_draw_blit2(context, surface, layerSurface, 0, 0, imageWidth - 1, imageHeight - 1, x, y);
	gf_draw_end(context);
	gf_context_disable_alpha(context);
	gf_surface_free(surface);
	return 0;
}
