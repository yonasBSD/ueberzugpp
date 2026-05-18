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

#include "os.hpp"
#include "process.hpp"
#include "tmux.hpp"
#include "util.hpp"

#include <fmt/format.h>
#include <sys/types.h>

#include <AvailabilityMacros.h>

// PROC_PIDT_SHORTBSDINFO was introduced in macOS 10.7
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1070
#include <libproc.h>
#define HAVE_PROC_PIDINFO
#else
#include <sys/sysctl.h>
#include <sys/param.h>
#endif

Process::Process(int pid)
    : pid(pid)
{
#ifdef HAVE_PROC_PIDINFO
    struct proc_bsdshortinfo sproc;
    struct proc_bsdinfo proc;

    int status = proc_pidinfo(pid, PROC_PIDT_SHORTBSDINFO, 0, &sproc, PROC_PIDT_SHORTBSDINFO_SIZE);
    if (status == PROC_PIDT_SHORTBSDINFO_SIZE) {
        ppid = static_cast<int>(sproc.pbsi_ppid);
    }

    status = proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc, PROC_PIDTBSDINFO_SIZE);
    if (status == PROC_PIDTBSDINFO_SIZE) {
        tty_nr = static_cast<int>(proc.e_tdev);
        minor_dev = minor(tty_nr);
        pty_path = fmt::format("/dev/ttys{:0>3}", minor_dev);
    }
#else
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, pid};
    struct kinfo_proc kp;
    size_t size = sizeof(kp);

    if (sysctl(mib, 4, &kp, &size, NULL, 0) == 0 && size == sizeof(kp)) {
        ppid = static_cast<int>(kp.kp_eproc.e_ppid);
        tty_nr = static_cast<int>(kp.kp_eproc.e_tdev);
        minor_dev = minor(tty_nr);
        pty_path = fmt::format("/dev/ttys{:0>3}", minor_dev);
    }
#endif
}
