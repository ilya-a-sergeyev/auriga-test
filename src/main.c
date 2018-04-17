#include "auriga_test.h"

/* constant default value */
const int quality=90;
const J_DCT_METHOD dct_method = JDCT_FASTEST;

int main (int argc, char *argv[])
{
	int err_code = EXIT_FAILURE;
	void *log_buf;
	int i;

	struct stat file_info;
	int fd;
	FILE *fout;

	unsigned long jpg_size;
	unsigned char *jpg_buffer;
	struct jpeg_decompress_struct cininfo;
	struct jpeg_compress_struct coutinfo;
	struct jpeg_error_mgr jerr;
	unsigned long bmp_size;
	JSAMPLE *bmp_buffer;
	JDIMENSION width, height;
	J_COLOR_SPACE colorspace;
	int pixel_size;

	log_buf = log_init(argv[0]);
	if (!log_buf) {
		goto exit;
	}

	if (parse_av(argc, argv)) {
		goto exit;
	}

	if (stat(in_filename, &file_info)) {
		log_err("FAILED to stat source filename : %s\n", in_filename);
		goto exit;
	}

	if ((fout = fopen(out_filename, "wb")) == NULL) {
	    log_err("FAILED to open destination filename : %s\n", out_filename);
	    goto exit;
	}

	jpg_size = file_info.st_size;
	jpg_buffer = (unsigned char*) malloc(jpg_size + 100);

	log_inf("Reading data");

	fd = open(in_filename, O_RDONLY);
	i = 0;
	while (i < jpg_size) {
		i +=read(fd, jpg_buffer + i, jpg_size - i);
	}
	close(fd);

	cininfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cininfo);

	log_inf("Set memory buffer as source\n");
	jpeg_mem_src(&cininfo, jpg_buffer, jpg_size);

	log_inf("Reading the JPEG header\n");

	if ( jpeg_read_header(&cininfo, TRUE) != 1) {
		log_err( "Bad input file format\n");
		free(jpg_buffer);
		goto exit;
	}

	log_inf( "Starting decompression\n");

	jpeg_start_decompress(&cininfo);

	width = cininfo.output_width;
	height = cininfo.output_height;
	pixel_size = cininfo.output_components;
	colorspace = cininfo.jpeg_color_space;

	log_inf("Input JPEG is %dx%d [%d]\n",  (int)width, (int)height, (int)pixel_size);

	bmp_size = width * pixel_size;
	bmp_buffer = (JSAMPLE *) malloc(bmp_size);

	log_inf( "Starting compression\n");

	coutinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&coutinfo);
	jpeg_stdio_dest(&coutinfo, fout);

	/*
	 * Let's take default values from source image
	 * it can be added to argv parsing function as parameters in the future
	*/
	coutinfo.image_width = width;
	coutinfo.image_height = height;
	coutinfo.input_components = pixel_size;
	coutinfo.in_color_space = JCS_RGB;

	log_inf("Output JPEG is %dx%d [%d]\n",  (int)width, (int)height, (int)pixel_size);

	jpeg_set_defaults(&coutinfo);

	jpeg_set_colorspace(&coutinfo, colorspace);

	jpeg_set_quality(&coutinfo, quality, TRUE);
	coutinfo.dct_method = dct_method;

	jpeg_start_compress(&coutinfo, TRUE);

	log_inf("Scanning and encoding lines\n");

	while (cininfo.output_scanline < cininfo.output_height) {
		JSAMPLE *buffer_array[1];
		buffer_array[0] = bmp_buffer;
		jpeg_read_scanlines(&cininfo, buffer_array, 1);
		(void) jpeg_write_scanlines(&coutinfo, buffer_array, 1);
	}

	log_inf( "Finishing decompression\n");

	jpeg_finish_decompress(&cininfo);
	jpeg_destroy_decompress(&cininfo);

	log_inf( "Finishing compression\n");

	jpeg_finish_compress(&coutinfo);
	fclose(fout);
	jpeg_destroy_compress(&coutinfo);

	free(bmp_buffer);
	free(jpg_buffer);

	log_inf( "Complete\n");
	err_code = EXIT_SUCCESS;

exit:
	log_destroy(log_buf);
	return err_code;
}
