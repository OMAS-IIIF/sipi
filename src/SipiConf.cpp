/*
 * Copyright © 2016 Lukas Rosenthaler, Andrea Bianco, Benjamin Geer,
 * Ivan Subotic, Tobias Schweizer, André Kilchenmann, and André Fatton.
 * This file is part of Sipi.
 * Sipi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Sipi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * Additional permission under GNU AGPL version 3 section 7:
 * If you modify this Program, or any covered work, by linking or combining
 * it with Kakadu (or a modified version of that library) or Adobe ICC Color
 * Profiles (or a modified version of that library) or both, containing parts
 * covered by the terms of the Kakadu Software Licence or Adobe Software Licence,
 * or both, the licensors of this Program grant you additional permission
 * to convey the resulting work.
 * See the GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public
 * License along with Sipi.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SipiConf.h"
#include <thread>

namespace Sipi {
    SipiConf::SipiConf() {}

    SipiConf::SipiConf(shttps::LuaServer &luacfg) {
        userid_str = luacfg.configString("sipi", "userid", "");
        hostname = luacfg.configString("sipi", "hostname", "localhost");
        port = luacfg.configInteger("sipi", "port", 3333);
#ifdef SHTTPS_ENABLE_SSL
        ssl_port = luacfg.configInteger("sipi", "ssl_port", -1);
        ssl_certificate = luacfg.configString("sipi", "ssl_certificate", "");
        ssl_key = luacfg.configString("sipi", "ssl_key", "");
#endif
        img_root = luacfg.configString("sipi", "imgroot", ".");
        max_temp_file_age = luacfg.configInteger("sipi", "max_temp_file_age", 86400);
        subdir_levels = luacfg.configInteger("sipi", "subdir_levels", 0);
        subdir_excludes = luacfg.configStringList("sipi", "subdir_excludes"); // has no defaults, returns an empty vector if nothing is there
        prefix_as_path = luacfg.configBoolean("sipi", "prefix_as_path", true);
        jpeg_quality = luacfg.configInteger("sipi", "jpeg_quality", 80);

        std::map<std::string,std::string> default_scaling_quality = {
                {"jpeg", "medium"},
                {"tiff", "high"},
                {"png",  "high"},
                {"j2k",  "high"}
        };
        scaling_quality = luacfg.configStringTable("sipi", "scaling_quality", default_scaling_quality);
        init_script = luacfg.configString("sipi", "initscript", ".");
        std::string cachesize_str = luacfg.configString("sipi", "cachesize", "0");

        if (!cachesize_str.empty()) {
            size_t l = cachesize_str.length();
            char c = cachesize_str[l - 1];

            if (c == 'M') {
                cache_size = stoll(cachesize_str.substr(0, l - 1)) * 1024 * 1024;
            } else if (c == 'G') {
                cache_size = stoll(cachesize_str.substr(0, l - 1)) * 1024 * 1024 * 1024;
            } else {
                cache_size = stoll(cachesize_str);
            }
        }

        cache_dir = luacfg.configString("sipi", "cachedir", "");
        cache_hysteresis = luacfg.configFloat("sipi", "cache_hysteresis", 0.1);
        keep_alive = luacfg.configInteger("sipi", "keep_alive", 20);
        thumb_size = luacfg.configString("sipi", "thumb_size", "!128,128");
        cache_n_files = luacfg.configInteger("sipi", "cache_nfiles", 0);
        n_threads = luacfg.configInteger("sipi", "nthreads", 2 * std::thread::hardware_concurrency());
        std::string max_post_size_str = luacfg.configString("sipi", "max_post_size", "0");

        if (!max_post_size_str.empty()) {
            size_t l = max_post_size_str.length();
            char c = max_post_size_str[l - 1];

            if (c == 'M') {
                max_post_size = stoll(max_post_size_str.substr(0, l - 1)) * 1024 * 1024;
            } else if (c == 'G') {
                max_post_size = stoll(max_post_size_str.substr(0, l - 1)) * 1024 * 1024 * 1024;
            } else {
                max_post_size = stoll(max_post_size_str);
            }
        }

        tmp_dir = luacfg.configString("sipi", "tmpdir", "/tmp");
        scriptdir = luacfg.configString("sipi", "scriptdir", "./scripts");
        jwt_secret = luacfg.configString("sipi", "jwt_secret", "");
        knora_path = luacfg.configString("sipi", "knora_path", "localhost");
        knora_port = luacfg.configString("sipi", "knora_port", "3333");
        loglevel = luacfg.configString("sipi", "loglevel", "WARN");
        logfile = luacfg.configString("sipi", "logfile", "sipi.log");
        adminuser = luacfg.configString("admin", "user", "");
        password = luacfg.configString("admin", "password", "");
        routes = luacfg.configRoute("routes");
        docroot = luacfg.configString("fileserver", "docroot", "");
        wwwroute = luacfg.configString("fileserver", "wwwroute", "");
    }

}
