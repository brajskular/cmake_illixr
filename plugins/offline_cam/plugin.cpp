#include "data_loading.hpp"
#include "illixr/opencv_data_types.hpp"
#include "illixr/phonebook.hpp"
#include "illixr/relative_clock.hpp"
#include "illixr/threadloop.hpp"

#include <chrono>
#include <shared_mutex>
#include <thread>

using namespace ILLIXR;
// __attribute__((visibility("default")))
extern "C" {    
    void start_roi_sam();
    void end_roi_sam();
}

    void start_roi_sam() {
        printf("sam started roi\n");
    }

    void end_roi_sam() {
        printf("sam ended roi\n");
    }

class offline_cam : public threadloop {
public:
    
    int sim_img_count;
    bool flag = false;

    offline_cam(const std::string& name_, phonebook* pb_)
        : threadloop{name_, pb_}
        , sb{pb->lookup_impl<switchboard>()}
        , _m_cam_publisher{sb->get_writer<cam_type>("cam")}
        , _m_sensor_data{load_data()}
        , dataset_first_time{_m_sensor_data.cbegin()->first}
        , last_ts{0}
        , _m_rtc{pb->lookup_impl<RelativeClock>()}
        , next_row{_m_sensor_data.cbegin()} { }

    skip_option _p_should_skip() override {
        if (true) {
            return skip_option::run;
        } else {
            return skip_option::stop;
        }
    }

    void _p_one_iteration() override {
        duration time_since_start = _m_rtc->now().time_since_epoch();
        // duration begin            = time_since_start;
        ullong lookup_time = std::chrono::nanoseconds{time_since_start}.count() + dataset_first_time;
        std::map<ullong, sensor_types>::const_iterator nearest_row;

        // "std::map::upper_bound" returns an iterator to the first pair whose key is GREATER than the argument.
        auto after_nearest_row = _m_sensor_data.upper_bound(lookup_time);

        if (after_nearest_row == _m_sensor_data.cend()) {
#ifndef NDEBUG
            std::cerr << "Running out of the dataset! "
                      << "Time " << lookup_time << " (" << _m_rtc->now().time_since_epoch().count() << " + "
                      << dataset_first_time << ") after last datum " << _m_sensor_data.rbegin()->first << std::endl;
#endif
            // Handling the last camera images. There's no more rows after the nearest_row, so we set after_nearest_row
            // to be nearest_row to avoiding sleeping at the end.
            nearest_row       = std::prev(after_nearest_row, 1);
            after_nearest_row = nearest_row;
            // We are running out of the dataset and the loop will stop next time.
            internal_stop();
        } else if (after_nearest_row == _m_sensor_data.cbegin()) {
            // Should not happen because lookup_time is bigger than dataset_first_time
#ifndef NDEBUG
            std::cerr << "Time " << lookup_time << " (" << _m_rtc->now().time_since_epoch().count() << " + "
                      << dataset_first_time << ") before first datum " << _m_sensor_data.cbegin()->first << std::endl;
#endif
        } else {
            // Most recent
            nearest_row = std::prev(after_nearest_row, 1);
        }

        if (last_ts != nearest_row->first) {
            last_ts = nearest_row->first;

            auto img0 = nearest_row->second.cam0.load();
            auto img1 = nearest_row->second.cam1.load();




            time_point expected_real_time_given_dataset_time(
                std::chrono::duration<long, std::nano>{nearest_row->first - dataset_first_time});

            sim_img_count++;
            // need to log time from start roi to end roi
            if(sim_img_count == 100) {
                start_roi_sam();
                flag = true;
                // start timer
                // start_point = std::chrono::high_resolution_clock::now();
            } else if(sim_img_count == 120) {
                end_roi_sam();
                flag = false;
                // end timer
                // end_point = std::chrono::high_resolution_clock::now();
                // print time elapsed
                // std::cout << "Time difference (sec) = " <<  (std::chrono::duration_cast<std::chrono::microseconds>(end_point - start_point).count()) /1000000.0  <<std::endl;
            }
            if(flag) {
                std::cout << "In roi" << std::endl;
            }
            _m_cam_publisher.put(_m_cam_publisher.allocate<cam_type>(cam_type{
                expected_real_time_given_dataset_time,
                img0,
                img1,
            }));
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(after_nearest_row->first - dataset_first_time -
                                                             _m_rtc->now().time_since_epoch().count() - 2));
    }

private:
    const std::shared_ptr<switchboard>             sb;
    switchboard::writer<cam_type>                  _m_cam_publisher;
    const std::map<ullong, sensor_types>           _m_sensor_data;
    ullong                                         dataset_first_time;
    ullong                                         last_ts;
    std::shared_ptr<RelativeClock>                 _m_rtc;
    std::map<ullong, sensor_types>::const_iterator next_row;
};

PLUGIN_MAIN(offline_cam)
