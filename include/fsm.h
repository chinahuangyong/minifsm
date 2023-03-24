//
// Created by HuangYong on 2023/3/22.
//

#ifndef __FSM_H__
#define __FSM_H__

#include <string>
#include <functional>
#include <memory>
#include <mutex>
#include <chrono>

namespace minifsm{
    class Event;
    class State;
    class FSM;

    class State {
    public:
        State();
        explicit State(std::string name);
        ~State();
        void set_name(std::string name);
        void get_name(std::string& name);
        uint64_t get_id() const;
        void register_enter_callback(std::function<void(void)> func);
        void register_exit_callback(std::function<void(void)> func);
        void register_run_callback(std::function<void(void)> func);
        void register_events(Event* event);
        void reset_event();
        std::vector<Event*> get_register_event();
        void translate(Event* event);
        State* run();

    private:
        std::string name_;
        uint64_t  id_;
        uint64_t event_count_ = UINT64_MAX;
        std::vector<Event*> v_event_ptr_;

        bool is_first_run_enter_cb_ = true;
        bool is_run_exit_cb_ = false;

        std::function<void(void)> enter_fun_cb_;
        std::function<void(void)> exit_fun_cb_;
        std::function<void(void)> run_fun_cb_;
    };

    class Event {
    public:
        Event();
        explicit Event(std::string name);
        ~Event();
        void set_name(std::string name);
        void get_name(std::string& name);
        uint64_t get_id() const;
        void set_event_number(uint64_t event_number_);
        void reset_event_number();
        uint64_t get_event_number() const;
        void register_next_state(State* state);
        State* get_next_state();

    private:
        std::string name_;
        uint64_t id_;
        uint64_t event_number_ = UINT64_MAX;
        State* next_state_ptr_;
    };

    class FSM {
    public:
        FSM();
        explicit FSM(State* state);
        ~FSM();
        void add_init_state(State* state);
        void add_state(State* state);
        void translate(Event* event);
        bool start();
    private:
        std::mutex mtx_;
        State* current_state_ptr_;
        std::vector<State*> v_state_ptr_;
    };
}

#endif //__FSM_H__
