#include "pngFile.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define ERROR 0
#define NOT_PNG 0

// conan package
#include "png.h"
#include "pnglibconf.h"

#define DATA_SIZE  (1024 * 1024)
char read_data[DATA_SIZE];
#define PNG_BYTES_TO_CHECK 8        // 检查是不是png文件

typedef struct {
    uint32_t height;
    uint32_t width;
    bool alpha;
    uint8_t type;
    uint8_t* pixel_data;
} IMG_ITEM;

int is_not_png_file(FILE **fp, const char *filename) //检查是否png文件
{
	char checkheader[PNG_BYTES_TO_CHECK]; //查询是否png头
	*fp = fopen(filename, "rb");
	if (*fp == NULL) {
		printf("open failed ...1\n");
		return 0;
	}
	if (fread(checkheader, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK) //读取png文件长度错误直接退出
		return 0;
	return png_sig_cmp(checkheader, 0, PNG_BYTES_TO_CHECK); //0正确, 非0错误
}

void testPngFile()
{
    printf("void testPngFile();\r\n");
}

// 在读取每行之后调用，可以使用它进行控制进度计或类似的东西。
/**
 * 
 * the 'row' and 'pass' refer to the next row to be handled
 */
void read_row_callback(png_structp png_ptr,
    png_uint_32 row, int pass)
{
    /* put your code here */
    //printf("111\n");
    return;
}

bool readPngFile(const char *file_name)
{
    IMG_ITEM *img_info = malloc(sizeof(IMG_ITEM));
    int img_all_pixels = 0;
#if (1)
    FILE *fp = fopen(file_name, "rb");
    int number = 7;
    if (!fp)
    {
        printf("open file failed\n");
        return ERROR;
    }

    if (fread(read_data, 1, number, fp) != number)
    {
        printf("read data file failed\n");
        return ERROR;
    }

    bool is_png = !png_sig_cmp(read_data, 0, number);
    if (!is_png)
    {
        printf("is not png file\n");
        return NOT_PNG;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        printf("create png struct faild\n");
       return ERROR;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
    {
       png_destroy_read_struct(&png_ptr,
           (png_infopp)NULL, (png_infopp)NULL);
        printf("info faild\n");
       return ERROR;
    }

    // 设置png大小的极限，下面数值是默认值2^31-1 (0x7fffffff)
    //png_set_user_limits(png_ptr, 0x7fffffff, 0x7fffffff);

    // 
    if (setjmp(png_jmpbuf(png_ptr)))
    {
       png_destroy_read_struct(&png_ptr, &info_ptr,
           0);
       fclose(fp);
       return ERROR;
    }
    rewind(fp); //等价fseek(fp, 0, SEEK_SET);

    // 确保文件是以二进制模式打开。
    png_init_io(png_ptr, fp);
    // 告诉png我们已经读取了多少字节了
    png_set_sig_bytes(png_ptr, number);
    
    //  设置使用libpng读取图像的buffer大小，默认为8192字节。
    //int buffer_size = 8192;
    //png_set_compression_buffer_size(png_ptr, buffer_size);

    // 设置每读取一行的回调函数
    //png_set_read_status_fn(png_ptr, read_row_callback);
#define hilevel
#ifdef hilevel
    // 开始读取数据： PNG_TRANSFORM_IDENTITY是原始格式读取
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
#else 
    png_read_info(png_ptr, info_ptr);
#endif
    // 完成读取就可以遍历了
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    int channels, color_type, depth, width, height; 
    channels = png_get_channels(png_ptr, info_ptr); //通道数量
    color_type = png_get_color_type(png_ptr, info_ptr);//颜色类型
    depth = png_get_bit_depth(png_ptr, info_ptr);// 深度

    width = png_get_image_width(png_ptr, info_ptr);// 深度
    height = png_get_image_height(png_ptr, info_ptr);// 深度
#if 0
    printf("Channels: %d\n", channels);
    printf("color_type: %d\n", color_type);
    printf("depth: %d\n", depth);
    printf("width: %d\n", width);
    printf("height: %d\n", height);
#endif
    img_info->type = color_type;
    img_info->width = width;
    img_info->height = height;
    for (int i = 0; i < width * height; i++)
    {
        printf("%d ", row_pointers[i]);
    }

    int image_size = width * height;
    // 24位的rgb
    if (channels == 3 || color_type == PNG_COLOR_TYPE_RGB) {
        img_info->alpha = false;
        img_all_pixels = height * width * 3;
        img_info->pixel_data = (png_bytep)malloc(img_all_pixels);
        if (NULL == img_info->pixel_data) {
            printf("Failed to allocate\r\n");
            png_destroy_read_struct(&png_ptr, &info_ptr, 0);
			fclose(fp);
            return false;
        }
        int temp = channels - 1;
        int pos = 0;
        int k = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width * 3; j++) {
                for (k = temp; k >= 0; k--) {
                    img_info->pixel_data[pos++] = row_pointers[i][j + k];
                }
            }
        }

        for (int i = 0; i < height * width * 3; i++) {
            printf("%d ", img_info->pixel_data[i]);
            if (i % 100 == 0) {
                printf("\n");
            }
        }

    }

#if (0)
    // 其中行指针是指向每行像素数据的指针数组:
    row_pointers = png_malloc(png_ptr, height * (sizeof (png_bytep)));

    for (int i = 0; i < height; i++) {
      row_pointers[i]=NULL;  /* security precaution */
   }

    int pixel_size = depth;
    for (int i = 0; i < height; i++) {
      row_pointers[i]=png_malloc(png_ptr,
          width*pixel_size);
   }
#endif

    png_set_rows(png_ptr, info_ptr, &row_pointers);


    free(img_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    fclose(fp);
#endif
    return true;


}