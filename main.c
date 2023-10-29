#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define BUFFERS_MAX_SIZE	256

void str_upr(const char *in, char *out){
	int len, i;
	
	len = strlen(in);
	
	for(i = 0; i < len; i++){
		out[i] = toupper(in[i]);
	}
}

void str_lwr(const char *in, char *out){
	int len, i;
	
	len = strlen(in);
	
	for(i = 0; i < len; i++){
		out[i] = tolower(in[i]);
	}
}

void str_extract_name(const char *in, char *out){
	int i, j;
	int last_slash_index = 0;
	char *last;
	int string_length;
	
	last = strrchr(in, '/');
	last_slash_index = last - in;
	
	string_length = strlen(in);
	
	for(i = last_slash_index+1, j = 0; i < string_length; i++, j++){
		if(in[i] == '.' || in[i] == '\0'){
			break;
		}
		out[j] = in[i];
	}
	
	out[i] = '\0';
}

int main(int argc, char **argv){
	FILE *fin, *fout;
	long int file_size;
	long int result;
	char in_file_name_str[BUFFERS_MAX_SIZE+1];
	char out_file_name_str[BUFFERS_MAX_SIZE+1];	
	uint8_t *fin_buff; 
	int i;
	char c_header_define_start[BUFFERS_MAX_SIZE+64];
	char c_header_define[BUFFERS_MAX_SIZE];
	char c_header_include[BUFFERS_MAX_SIZE];
	char c_array_declaration_start[BUFFERS_MAX_SIZE];
	char byte_str[BUFFERS_MAX_SIZE];
	char line_str[BUFFERS_MAX_SIZE];
	char c_array_declaration_end[BUFFERS_MAX_SIZE];
	char c_header_define_end[BUFFERS_MAX_SIZE];
	char in_upper_str[BUFFERS_MAX_SIZE/2+1];
	char in_lower_str[BUFFERS_MAX_SIZE/2+1];
	char out_file_name_proc_str[BUFFERS_MAX_SIZE+1];
	
	if(argc < 3){
		printf("\nUsage: bin_to_array [path_and_file_name_in] [path_and_file_name_out]\n\n"\
				"path_and_file_name_in - the binary file to be converted to a C array (bin, jpg, png, pdf, etc).\n"
				"path_and_file_name_out - the text file that will hold an array of bytes in C.\n\n"
				"Example:\n"\
				"bin_to_array /home/user/Desktop/pic.jpg /home/user/Desktop/pic.h\n"\
				);
		return EXIT_FAILURE;
	}

	strncpy(in_file_name_str, argv[1], BUFFERS_MAX_SIZE);
	strncpy(out_file_name_str, argv[2], BUFFERS_MAX_SIZE);

	fin = fopen(in_file_name_str, "rb");

	if(fin == NULL){
		printf("Error opening file %s!\n", in_file_name_str);
		return EXIT_FAILURE;
	}

	printf("File %s opened successfully!\n", in_file_name_str);

	fseek(fin, 0L, SEEK_END);
	file_size = ftell(fin);
	rewind(fin);

	printf("File size is: %ld bytes\n", file_size);
	
	fin_buff = (uint8_t *)malloc(file_size+1);	
	
	if(fin_buff == NULL){
		printf("Memory allocation error!\n");
		return EXIT_FAILURE;
	}

	result = fread(fin_buff, 1, file_size, fin);
	
	printf("File buffering result: %ld bytes\n", result);

	fclose(fin);

	fout = fopen(out_file_name_str, "w");

	if(fout == NULL){
		printf("Error writing to file %s!\n", out_file_name_str);
		return EXIT_FAILURE;
	}
	
	printf("File %s opened successfully!\n", out_file_name_str);
	
	str_extract_name(out_file_name_str, out_file_name_proc_str);
	
	printf("Assigning array name: %s\n\r", out_file_name_proc_str);
	
	str_upr(out_file_name_proc_str, in_upper_str);	
	str_lwr(out_file_name_proc_str, in_lower_str);
	
	snprintf(c_header_define_start, BUFFERS_MAX_SIZE, "#ifndef __%s_H_\n", in_upper_str);
	snprintf(c_header_define, BUFFERS_MAX_SIZE, "#define __%s_H_\n\n", in_upper_str);
	strcpy(c_header_include, "#include <stdint.h>\n\n");
	snprintf(c_array_declaration_start, BUFFERS_MAX_SIZE, "const uint8_t %s[%ld] = {\n", in_lower_str, file_size);
	
	fwrite(c_header_define_start, 1, strlen(c_header_define_start), fout);
	fwrite(c_header_define, 1, strlen(c_header_define), fout);
	fwrite(c_header_include, 1, strlen(c_header_include), fout);
	fwrite(c_array_declaration_start, 1, strlen(c_array_declaration_start), fout);
	
	strcpy(line_str, "");
	
	for(i = 1; i < file_size; i++){
		sprintf(byte_str, "0x%02x, ", fin_buff[i-1]);
		strcat(line_str, byte_str);
		
		if((i % 16 == 0) && (i != 0)){
			strcat(line_str, "\n");
			fwrite(line_str, 1, strlen(line_str), fout);
			strcpy(line_str, "");
		}
	}
	
	strcpy(c_array_declaration_end, "};\n\n");
	strcpy(c_header_define_end, "#endif\n");
	
	fwrite(c_array_declaration_end, 1, strlen(c_array_declaration_end), fout);
	fwrite(c_header_define_end, 1, strlen(c_header_define_end), fout);

	fflush(fout);	

	fclose(fout);
	
	printf("Conversion completed successfully!\n");
	
	return EXIT_SUCCESS;
}
