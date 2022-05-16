#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__
//Recomendation:- Dont modify this file.
#define USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWfdgebKit/537.36 (KHTML, like Gecko) Chrome/99 Safari/537.36" //User agent
#define sleep_time_ms 15// time in milliseconds for which we sleep after every request
#define max_num_item 100// maximum number of items in a single database
#define datadir "/data"// directory where the dataset is stored
#define cache_dir "/cache"// directory where the cache is stored
#define referance_dir "/reference"// directory where the referance is stored
#define cache_url_file cache_dir"/cache.url"// file where the cache url is stored
#define referance_file referance_dir"/reference.rdb"// file where the referance is stored
#endif