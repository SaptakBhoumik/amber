//g++  test.cc -lboost_iostreams -o test
#include <iostream>
#include <string>
#include <sstream>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zstd.hpp>


std::string compress(std::string& data)
    {
        namespace bio = boost::iostreams;

        std::stringstream compressed;
        std::stringstream origin(data);

        bio::filtering_streambuf<bio::input> out;
        out.push(bio::zstd_compressor(bio::zstd_params(bio::zstd::default_compression)));

        out.push(origin);
        bio::copy(out, compressed);

        return compressed.str();
}

std::string decompress(std::string& data)
    {
      namespace bio = boost::iostreams;

      std::stringstream decompressed;
      std::stringstream origin(data);

      bio::filtering_streambuf<bio::input> out;
       out.push(bio::zstd_decompressor(bio::zstd_params(bio::zstd::default_compression)));
      out.push(origin);
      bio::copy(out, decompressed);

      return decompressed.str();
}


int main(int argc, char *argv[])
{

  std::string original = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  std::string compressed_encoded = compress(original);
  std::cout << compressed_encoded << std::endl;
  std::string decompressed_decoded = decompress(compressed_encoded);
  std::cout << decompressed_decoded << std::endl;
  if (original == decompressed_decoded) {
    std::cout << "Successfully compressed/decompressed\n";
  }
  return 0;
}
