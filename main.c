#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <sys/stat.h>
#include <bitset>


struct PPMHeader {
   char type[2];
   int width, height;
   int max_color_value;
   int size;
};

void reveal(std::ifstream& filestream, std::ofstream& output_filestream);
void hide(std::ifstream& filestream, std::ifstream& filestream_to_hide, std::ofstream& output_filename);
size_t getStreamSize(std::ifstream&);
struct PPMHeader getHeader(std::ifstream& filestream);
void dumpHeader(PPMHeader&);


void usage(const char* program_name) {
   std::cout << "Hide/reveal ppm image in another ppm image" << std::endl
             << "Usage: " << program_name << " [action] [options]" << std::endl
             << "\thide dst src output_filename - encode an image into another" << std::endl
             << "\treveal dst output_filename - reveal a hidden image" << std::endl;
}


int main(int argc, char **argv) {
   if(argc < 3) {
      usage(argv[0]);
      return EXIT_FAILURE;
   }
   std::string action = std::string(argv[1]);
   std::ifstream filestream(argv[2], std::ios::in | std::ios::binary);
   if(action.compare("hide") == 0) {
      std::ifstream filestream_to_hide(argv[3], std::ios::in | std::ios::binary);
      std::ofstream output_filestream(argv[4], std::ios::out | std::ios::binary);
      std::cout << "writing to " << argv[4];
      if(argc < 5) {
         usage(argv[0]);
         return EXIT_FAILURE;
      }
      hide(filestream, filestream_to_hide, output_filestream);
   } else if(action.compare("reveal") == 0) {
      std::ofstream output_filestream(argv[4], std::ios::out | std::ios::binary);
      if(argc < 4) {
         usage(argv[0]);
         return EXIT_FAILURE;
      }
      reveal(filestream, output_filestream);
   } else {
      std::cout << "no action named \"" << action << "\"" << std::endl;
      usage(argv[0]);
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}


void reveal(std::ifstream& filestream, std::ofstream& output_filestream) {
   if(filestream.is_open()) {
      std::cout << "file is open" << std::endl;
   } 
}

void hide(std::ifstream& filestream, std::ifstream& filestream_to_hide, std::ofstream& output_filestream) {
   char native_binary_char, binary_to_hide_char;
   const size_t stream_to_hide_size = getStreamSize(filestream_to_hide);
   const size_t native_stream_size = getStreamSize(filestream);
   struct PPMHeader header = getHeader(filestream);
   std::bitset<8> hidden_binary, native_binary, binary_to_hide;
   std::bitset<2> subset_to_hide;
   dumpHeader(header);
   output_filestream << header.type << std::endl 
                     << header.width << " " << header.height << std::endl
                     << header.max_color_value;
   // Hide resolution of image to hide
   for(size_t s=0; s < stream_to_hide_size; s++) {
      filestream_to_hide.get(binary_to_hide_char);
      binary_to_hide = binary_to_hide_char;
      //std::cout << std::endl << "Hiding " << binary_to_hide << std::endl;
      for(int i=0; i < 8; i+=2) {
         filestream.get(native_binary_char);
         hidden_binary = native_binary_char;
         subset_to_hide[0] = binary_to_hide[i];
         subset_to_hide[1] = binary_to_hide[i + 1];
         hidden_binary[0] = binary_to_hide[i];
         hidden_binary[1] = binary_to_hide[i + 1];
         output_filestream << char(hidden_binary.to_ulong());
      }
   }
   // Fill the rest of the file
   for(size_t s=stream_to_hide_size; s < native_stream_size; s++) {
      filestream.get(native_binary_char);
      output_filestream << native_binary_char;
   }
}

size_t getStreamSize(std::ifstream& filestream) {
   const size_t current_position = filestream.tellg();
   filestream.seekg(0, filestream.end);
   const size_t filesize = filestream.tellg();
   filestream.seekg(current_position, filestream.beg);
   return filesize;
}

struct PPMHeader getHeader(std::ifstream& filestream) {
   struct PPMHeader header;
   char width_buf[4], height_buf[4], max_ascii_val_buf[32];
   filestream >> header.type >> width_buf >> height_buf >> max_ascii_val_buf;
   header.width = std::atoi(width_buf);
   header.height = std::atoi(height_buf);
   header.max_color_value = std::atoi(max_ascii_val_buf);
   return header;
}

void dumpHeader(struct PPMHeader& header) {
   std::cout << "Colors: ";
   if(std::string("P6").compare(header.type) == 0) {
      std::cout << "binary";
   } else if(std::string("P3").compare(header.type) == 0) {
      std::cout << "ASCII";
   } else {
      std::cout << "Unknown";
   }
   std::cout << " (" << header.type << ")" << std::endl;
   std::cout << "width x height: " << header.width << "x" << header.height << std::endl;
   std::cout << "max color value: " << header.max_color_value << std::endl;
}

void dumpBinary(std::ifstream& filestream) {
   // Must start at the beginning of the data
}
