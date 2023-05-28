// Display images inside a terminal
// Copyright (C) 2023  JustKidding
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef __SWAY_SHM__
#define __SWAY_SHM__

#include <wayland-client.h>
#include <string>
#include <vector>

class SwayShm
{
public:
    SwayShm(int width, int height, struct wl_shm* shm);
    ~SwayShm();
    auto get_data(uint32_t offset = 0) -> uint32_t*;

    struct wl_buffer* buffer = nullptr;
private:
    void create_shm_file();
    void allocate_pool_buffers();

    struct wl_shm_pool* pool = nullptr;
    struct wl_shm* shm = nullptr;

    int fd = 0;
    std::string shm_path;
    uint8_t *pool_data;
    std::vector<wl_buffer*> buffers;

    int width = 0;
    int height = 0;
    int stride = 0;
    int pool_size = 0;
};

#endif