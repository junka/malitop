#include <device/product_id.hpp>
#include <hwcpipe/counter_database.hpp>
#include <hwcpipe/gpu.hpp>
#include <hwcpipe/sampler.hpp>

#include <memory>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include <unistd.h>
#include <ncurses/ncurses.h>

class MaliGPUInfo {
private:
    hwcpipe::gpu gpu;
    std::map<hwcpipe_counter, const char *> threads = {
        {MaliFragThread, "Frag"},
        {MaliFragHelpThread, "FragHelp"},
        {MaliFragPrepassThread, "FragPre"},
        {MaliFragMainThread, "FragMain"},
        {MaliNonFragThread, "NonFrag"},
        {MaliGeomPosShadThread, "GeoPos"},
        {MaliGeomVarShadThread, "GeoVar"},
    };
    std::vector<hwcpipe_counter> enabled_threads;

    std::map<hwcpipe_counter, const char *> utilization = {
        {MaliGPUIRQUtil, "IRQ"},
        {MaliFragQueueUtil, "FragQ"},
        {MaliNonFragQueueUtil, "NonFragQ"},
        {MaliTilerUtil, "Tiler"},
        {MaliNonFragUtil, "NonFrag"},
        {MaliFragUtil, "Frag"},
        {MaliCoreUtil, "Core"},
        {MaliALUUtil, "ALU"},
        {MaliTexUtil, "Texture"},
        {MaliLSUtil, "LoadStore"},
        {MaliFragFPKBUtil, "FragFPKB"},
        {MaliTexCacheUtil, "TexCache"},
        {MaliVarUtil, "Variable"},
        {MaliEngFMAPipeUtil, "FMAPipe"},
        {MaliEngCVTPipeUtil, "CVTPipe"},
        {MaliEngSFUPipeUtil, "SFUPipe"},
        {MaliTexInBusUtil, "TexInBus"},
        {MaliTexOutBusUtil, "TexOutBus"},
        {MaliAnyUtil, "Any"},
        {MaliVertQueueUtil, "VertQ"},
        {MaliCompQueueUtil, "CompQ"},
        {MaliCSFMCUUtil, "CSFMCU"},
        {MaliCSFLSUUtil, "CSFLSU"},
        {MaliCSFCEUUtil, "CSFCEU"},
        {MaliRTUUtil, "RTU"},
        {MaliMainQueueUtil, "MainQ"},
        {MaliBinningQueueUtil, "BinningQ"},
        {MaliMainUtil, "Main"},
        {MaliCompOrBinningUtil, "CompOrBinning"},
    };
    std::vector<hwcpipe_counter> enabled_utilization;
    std::map<hwcpipe_counter, const char *> nonfrag = {
        {MaliNonFragQueueJob, "QJob"},
        {MaliNonFragQueueTask, "QTask"},
        {MaliNonFragQueueActiveCy, "QActiveCy"},
        {MaliNonFragActiveCy, "ActiveCy"},
        {MaliNonFragThread, "Thread"},
        {MaliNonFragQueueUtil, "QUtil"},
        {MaliNonFragUtil, "Util"},
        {MaliNonFragThroughputCy, "ThrhputCy"},
        {MaliNonFragQueueWaitRdCy, "QWaitRdCy"},
        {MaliNonFragQueueWaitIssueCy, "QWaitIssCy"},
        {MaliNonFragQueueWaitDepCy, "QWaitDepCy"},
        {MaliNonFragQueueWaitFinishCy, "QWaitFinCy"},
        {MaliNonFragQueueWaitFlushCy, "QWaitFluCy"},
        {MaliNonFragTask, "Task"},
        {MaliNonFragWarp, "Warp"},
    };
    std::vector<hwcpipe_counter> enabled_nonfrag;

    std::map<hwcpipe_counter, const char *> comp = {
        {MaliCompQueuedCy, "QueuedCy"},
        {MaliCompQueueJob, "Job"},
        {MaliCompQueueTask, "Task"},
        {MaliCompQueueTotalActiveCy, "TotActCy"},
        {MaliCompQueueIRQActiveCy, "IRQActCy"},
        {MaliCompQueueAssignStallCy, "AsgnStall"},
        {MaliCompQueueDrainStallCy, "DrnStall"},
        {MaliCompQueueActiveCy, "ActiveCy"},
        {MaliCompQueueUtil, "Util"},
        {MaliCompOrBinningActiveCy, "BinActCy"},
        {MaliCompOrBinningUtil, "BinUtil"},
    };
    std::vector<hwcpipe_counter> enabled_comp;

    std::map<hwcpipe_counter, const char *> mmu = {
        {MaliMMUL2Hit, "L2Hit"},
        {MaliMMUL2Rd, "L2Read"},
        {MaliMMULookup, "Lookup"},
        {MaliMMUL3Rd, "L3Read"},
        {MaliMMUL3Hit, "L3Hit"},
        {MaliMMUS2Lookup, "S2Lookup"},
        {MaliMMUS2L3Rd, "S2L3Read"},
        {MaliMMUS2L2Rd, "S2L2Read"},
        {MaliMMUS2L3Hit, "S2L3Hit"},
        {MaliMMUS2L2Hit, "S2L2Hit"},
    };
    std::vector<hwcpipe_counter> enabled_mmu;

    std::map<hwcpipe_counter, const char *> l2cache = {
        {MaliL2CacheRd, "Read"},
        {MaliL2CacheWr, "Write"},
        {MaliL2CacheSnp, "Snp"},
        {MaliL2CacheL1Rd, "L1Read"},
        {MaliL2CacheL1Wr, "L1Write"},
        {MaliL2CacheSnpLookup, "SnpLookup"},
        {MaliL2CacheIncSnp, "IncSnp"},
        {MaliL2CacheRdMissRate, "RdMissRat"},
        {MaliL2CacheWrMissRate, "WrMissRat"},
        {MaliL2CacheEvict, "Evict"},
        {MaliL2CacheCleanUnique, "Clean"},
        {MaliL2CacheLookup, "Lookup"},
        {MaliL2CacheRdLookup, "RdLookup"},
        {MaliL2CacheWrLookup, "WrLookup"},
        {MaliL2CacheFlush, "Flush"},
    };
    std::vector<hwcpipe_counter> enabled_l2cache;

    std::map<hwcpipe_counter, const char *> extbus = {
        {MaliExtBusWrBt, "WrBt"},
        {MaliExtBusRdBt, "RdBt"},
        {MaliExtBusRd, "Read"},
        {MaliExtBusRdNoSnoop, "RdNoSnoop"},
        {MaliExtBusRdUnique, "RdUnique"},
        {MaliExtBusRdOTQ1, "OTQ1"},
        {MaliExtBusRdOTQ2, "OTQ2"},
        {MaliExtBusRdOTQ3, "OTQ3"},
        {MaliExtBusRdLat0, "Lat0"},
        {MaliExtBusRdLat128, "Lat128"},
        {MaliExtBusRdLat192, "Lat192"},
        {MaliExtBusRdLat256, "Lat256"},
        {MaliExtBusRdLat320, "Lat320"},
        {MaliExtBusRdLat384, "Lat384"},
        {MaliExtBusWr, "Write"},
        {MaliExtBusWrNoSnoopFull, "WrNoSnpF"},
        {MaliExtBusWrNoSnoopPart, "WrNoSnpP"},
        {MaliExtBusWrSnoopFull, "WrSnpFull"},
        {MaliExtBusWrSnoopPart, "WrSnpPart"},
        {MaliExtBusWrOTQ1, "WrOTQ1"},
        {MaliExtBusWrOTQ2, "WrOTQ2"},
        {MaliExtBusWrOTQ3, "WrOTQ3"},
        {MaliExtBusRdOTQ4, "RdOTQ4"},
        {MaliExtBusWrOTQ4, "WrOTQ4"},
    };
    std::vector<hwcpipe_counter> enabled_extbus;

    std::unique_ptr<hwcpipe::sampler<>> sampler_ptr;

    void setup_sampler() {
        hwcpipe::sampler_config config(gpu);
        std::error_code ec;

        auto enable_counter_func = [&config](std::map<hwcpipe_counter, const char *> &counters,
                                             std::vector<hwcpipe_counter> &enabled) {
            for (auto &c : counters) {
                auto ec = config.add_counter(c.first);
                if (ec) {
                    continue;
                }
                enabled.push_back(c.first);
            }
        };

        enable_counter_func(threads, enabled_threads);
        enable_counter_func(utilization, enabled_utilization);
        enable_counter_func(nonfrag, enabled_nonfrag);
        enable_counter_func(comp, enabled_comp);
        enable_counter_func(l2cache, enabled_l2cache);
        enable_counter_func(mmu, enabled_mmu);
        enable_counter_func(extbus, enabled_extbus);

        sampler_ptr = std::make_unique<hwcpipe::sampler<>>(config);
        ec = sampler_ptr->start_sampling();
        if (ec) {
            printw("%s\n", ec.message().c_str());
            return;
        }
    }
public:
    MaliGPUInfo(int id) {
        gpu = hwcpipe::gpu(id); 
        if (!gpu) {
            std::cerr << "Mali GPU device is missing" << std::endl;            
            throw std::runtime_error("Mali GPU device is missing");
        }
        setup_sampler();
    }
    ~MaliGPUInfo() {
        auto ec = sampler_ptr->stop_sampling();
        if (ec) {
            printw("%s\n", ec.message().c_str());
        }
    }

    void print_device_info() {
        printw("------------------------------------------------------------\n");
        auto ec = sampler_ptr->sample_now();
        if (ec) {
            printw("%s\n", ec.message().c_str());
            refresh();
            return;
        }
        auto print_func = [this](const hwcpipe_counter &c) {
            hwcpipe::counter_sample sample;
            std::error_code ec;
            ec = sampler_ptr->get_counter_value(c, sample);
            if (ec) {
                printw("%-16s\n", ec.message().c_str());
                return -1;
            }
            switch (sample.type) {
            case hwcpipe::counter_sample::type::uint64:
                printw(" %-9lu", sample.value.uint64);
                break;
            case hwcpipe::counter_sample::type::float64:
                printw(" %-9.02f", sample.value.float64);
                break;
            default:
                break;
            }
            return 0;
        };
        

        auto algined_print_func = [print_func](std::map<hwcpipe_counter, const char *> &counters,
                                               std::vector<hwcpipe_counter> &enabled) {
            printw("\n------------------------------------------------------------\n");

            int max_x;
            getmaxyx(stdscr, std::ignore, max_x);

            const int default_per_line = 12;
            int per_line = default_per_line;

            if (max_x > 0) {
                int element_width = 10; // Approximate width for each element (9 chars + 1 space)
                per_line = max_x / element_width;
                if (per_line > default_per_line) {
                    per_line = default_per_line;
                }
            }

            for (size_t i = 0; i < enabled.size(); i += per_line) {
                for (size_t j = i; j < i + per_line && j < enabled.size(); ++j) {
                    printw(" %-9s", counters.at(enabled[j]));
                }
                printw("\n");

                for (size_t j = i; j < i + per_line && j < enabled.size(); ++j) {
                    if (print_func(enabled[j])) {
                        continue;
                    }
                }
                printw("\n");
            }
            printw("------------------------------------------------------------\n");
        };
        if (!enabled_threads.empty()) {
            printw("Threads:");
            algined_print_func(threads, enabled_threads);
        }
        if (!enabled_utilization.empty()) {
            printw("Utilization:");
            algined_print_func(utilization, enabled_utilization);
        }
        if (!enabled_nonfrag.empty()) {
            printw("Non-Frag:");
            algined_print_func(nonfrag, enabled_nonfrag);
        }

        if (!enabled_comp.empty()) {
            printw("Comp:");
            algined_print_func(comp, enabled_comp);
        }
        if (!enabled_mmu.empty()) {
            printw("MMU:");
            algined_print_func(mmu, enabled_mmu);
        }
        if (!enabled_l2cache.empty()) {
            printw("L2 Cache:");
            algined_print_func(l2cache, enabled_l2cache);
        }
        if (!enabled_extbus.empty()) {
            printw("External Bus:");
            algined_print_func(extbus, enabled_extbus);
        }
        
        refresh();
    }
};


int main() {
    const char *term = getenv("TERM");
    if (!term) {
        std::cerr << "Environment variable TERM is not set." << std::endl;
        return -1;
    }

    std::string terminfo_path;
    if (access(("/usr/share/terminfo/" + std::string(1, term[0]) + "/" + term).c_str(), F_OK) == 0) {
        terminfo_path = "/usr/share/terminfo/";
    } else if (access(("/lib/terminfo/" + std::string(1, term[0]) + "/" + term).c_str(), F_OK) == 0) {
        terminfo_path = "/lib/terminfo/";
    } else {
        std::cerr << "Failed to find terminfo for terminal: " << term << std::endl;
        return -1;
    }

    if (setenv("TERMINFO", terminfo_path.c_str(), 1) != 0) {
        std::cerr << "Failed to set TERMINFO environment variable." << std::endl;
        return -1;
    }

    if (initscr() == nullptr) {
        std::cerr << "Failed to initialize ncurses screen" << std::endl;
        return -1;
    }
    cbreak();
    noecho();
    curs_set(0);

    try {
        using product_id = hwcpipe::device::product_id;
        std::map<product_id, const char *> product_map = {
            {product_id::t60x, "Mali-T60x"},
            {product_id::t62x, "Mali-T62x"},
            {product_id::t720, "Mali-T720"},
            {product_id::t760, "Mali-T760"},
            {product_id::t820, "Mali-T820"},
            {product_id::t830, "Mali-T830"},
            {product_id::t860, "Mali-T860"},
            {product_id::t880, "Mali-T880"},
            {product_id::g31, "Mali-G31"},
            {product_id::g51, "Mali-G51"},
            {product_id::g52, "Mali-G52"},
            {product_id::g71, "Mali-G71"},
            {product_id::g72, "Mali-G72"},
            {product_id::g76, "Mali-G76"},
            {product_id::g57, "Mali-G57"},
            {product_id::g57_2, "Mali-G57"},
            {product_id::g68, "Mali-G68"},
            {product_id::g77, "Mali-G77"},
            {product_id::g78, "Mali-G78"},
            {product_id::g78ae, "Mali-G78AE"},
            {product_id::g310, "Mali-G310"},
            {product_id::g510, "Mali-G510"},
            {product_id::g610, "Mali-G610"},
            {product_id::g615, "Mali-G615"},
            {product_id::g710, "Mali-G710"},
            {product_id::g715, "Mali-G715"},
            {product_id::g720, "Mali-G720"},
            {product_id::g620, "Mali-G620"},
            {product_id::g725, "Mali-G725"},
            {product_id::g625, "Mali-G625"},
        };

        while (true) {
            for (const auto &gpu : hwcpipe::find_gpus()) {
                clear();
                std::ostringstream oss;
                oss << gpu.get_gpu_family();
                std::string gpu_family_str = oss.str();
                printw("------------------------------------------------------------\n");
                printw(" GPU Device %d: %s, Arch: %s\n", gpu.get_device_number(),
                    product_map.find(gpu.get_product_id()) != product_map.end() ? product_map[gpu.get_product_id()] : "Unknown",
                    gpu_family_str.c_str());
                printw("------------------------------------------------------------\n");
                printw(" Cores: %ld, Engines %ld\n", gpu.num_shader_cores(), gpu.num_execution_engines());
                printw(" Bus Width: %ld\n", gpu.bus_width());
                
                MaliGPUInfo mali_gpu_info(gpu.get_device_number());
                mali_gpu_info.print_device_info();

                // Set up pselect for 1-second timeout and key press detection
                fd_set read_fds;
                FD_ZERO(&read_fds);
                FD_SET(STDIN_FILENO, &read_fds);

                struct timespec timeout;
                timeout.tv_sec = 1;
                timeout.tv_nsec = 0;

                int ret = pselect(STDIN_FILENO + 1, &read_fds, nullptr, nullptr, &timeout, nullptr);
                if (ret > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {
                    int ch = getch();
                    if (ch == 'q') {
                        endwin();
                        return 0;
                    }
                }
            }
        }
    } catch(const std::exception &e) {
        printw("Error: %s\n", e.what());
        refresh();
        getch();
        endwin();
        return -1;
    }

    endwin();
    return 0;
}

