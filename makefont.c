#include <stdio.h>

#define PNG_DEBUG 3
#include <png.h>

int width, height;
png_byte color_type;
png_byte bit_depth;
png_structp png_ptr;
png_infop info_ptr;
png_bytep *row_pointers;

int read_png_file(char *file_name)
{
	unsigned char header[8];	// 8 is the maximum size that can be checked
	int y;

	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
	{
		fprintf(stderr, "Can't read from %s", file_name);
		return 0;
	}

	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
	{
		fprintf(stderr, "%s is not recognized as a PNG file", file_name);
		return 0;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr)
	{
		fprintf(stderr, "png_create_read_struct failed");
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fprintf(stderr, "png_create_info_struct failed");
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fprintf(stderr, "Error during init_io");
		return 0;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = info_ptr->width;
	height = info_ptr->height;
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;

	png_read_update_info(png_ptr, info_ptr);

	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fprintf(stderr, "Error during read_image");
		return 0;
	}

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (y=0; y<height; y++)
		row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);

	png_read_image(png_ptr, row_pointers);

        fclose(fp);

	return 1;
}

int main(int argc, char *argv[])
{
	FILE *out;
	int i, x, y;

	if (argc != 3)
	{
		fprintf(stderr, "makefont <input.png> <output.c>\n");
		return 1;
	}

	if (!read_png_file(argv[1]))
		return 1;

	if (info_ptr->rowbytes != 128 || width != 1024 || height != 8 || bit_depth != 1)
	{
		fprintf(stderr, "Unexpected format - must be B/W 1024x8\n");
		return 1;
	}

	out = fopen(argv[2], "wt");
	if (!out)
	{
		fprintf(stderr, "Can't write to %s\n", argv[2]);
		return 1;
	}

	fprintf(out, "#include <avr/pgmspace.h>\n\n");
	fprintf(out, "prog_uchar font_data[] =\n{\n");

	for (i = 0, x = 0; x < 128 * 8; i++, x++)
	{
		unsigned char byte = 0;

		for (y = 0; y < 8; y++)
		{
			byte >>= 1;

			if (row_pointers[y][x / 8] & (0x80 >>(x & 7)))
				byte |= 0x80;
		}

		if ((i & 15) == 0)
			fprintf(out, "\t");

		fprintf(out, "0x%02x", byte ^ 0xff);

		if (x != (128 * 8 - 1))
			fprintf(out, ",");

		if ((i & 15) == 15)
			fprintf(out, "\n");
		else fprintf(out, " ");
	}

	fprintf(out, "};\n\n");

	fclose(out);

	return 0;
}

