#include <iostream>
#include <string>
#include <sstream>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zstd.hpp>
#include "../include/compress/compress.hpp"
namespace Amber{
std::string Compress::compress(std::string data){
        namespace bio = boost::iostreams;

        std::stringstream compressed;
        std::stringstream origin(data);

        bio::filtering_streambuf<bio::input> out;
        out.push(bio::zstd_compressor(bio::zstd_params(bio::zstd::default_compression)));

        out.push(origin);
        bio::copy(out, compressed);

        return compressed.str();
}

std::string Compress::decompress(std::string data)
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
}
