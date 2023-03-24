//
// Created by HuangYong on 2023/3/22.
//
// TODO: State可以按照ID进行排序存储，可以增加后续查找的时间，但是状态量比较少时意义不大；
// TODO: 采用范型编程调用回调函数
//

#include "fsm.h"

#include <iostream>
#include <utility>

namespace minifsm {
    Event::Event() : event_number_(UINT64_MAX){
        next_state_ptr_ = nullptr;
        id_ = std::chrono::steady_clock::now().time_since_epoch().count();
        std::cout << "event id: " << id_ << std::endl;
    }

    Event::Event(std::string name) : event_number_(UINT64_MAX) , name_(std::move(name)){
        next_state_ptr_ = nullptr;
        id_ = std::chrono::steady_clock::now().time_since_epoch().count();
        std::cout << "event id: " << id_ << std::endl;
    }

    Event::~Event() {
        next_state_ptr_ = nullptr;
        event_number_ = UINT64_MAX;
        id_ = 0;
    }

    void Event::set_name(std::string name) {
        name_ = std::move(name);
    }

    void Event::get_name(std::string& name) {
        name = name_;
    }

    uint64_t Event::get_id() const {
        return id_;
    }

    void Event::set_event_number(uint64_t event_number) {
        event_number_ = event_number;
    }

    void Event::reset_event_number() {
        event_number_ = UINT64_MAX;
    }

    uint64_t Event::get_event_number() const {
        return event_number_;
    }

    void Event::register_next_state(State *state) {
        next_state_ptr_ = state;
    }

    State* Event::get_next_state() {
        return next_state_ptr_;
    }

    State::State() {
        is_first_run_enter_cb_ = true;
        is_run_exit_cb_ = false;
        event_count_ = UINT64_MAX;
        id_ = std::chrono::steady_clock::now().time_since_epoch().count();
        std::cout << "state id: " << id_ << std::endl;

        enter_fun_cb_ = nullptr;
        exit_fun_cb_ = nullptr;
        run_fun_cb_ = nullptr;
    }

    State::~State() {
        is_first_run_enter_cb_ = true;
        is_run_exit_cb_ = false;
        event_count_ = UINT64_MAX;

        enter_fun_cb_ = nullptr;
        exit_fun_cb_ = nullptr;
        run_fun_cb_ = nullptr;
    }

    State::State(std::string name) : name_(std::move(name)){
        is_first_run_enter_cb_ = true;
        is_run_exit_cb_ = false;
        event_count_ = UINT64_MAX;
        id_ = std::chrono::steady_clock::now().time_since_epoch().count();
        std::cout << "state id: " << id_ << std::endl;

        enter_fun_cb_ = nullptr;
        exit_fun_cb_ = nullptr;
        run_fun_cb_ = nullptr;
    }

    void State::set_name(std::string name) {
        name_ = std::move(name);
    }

    void State::get_name(std::string &name) {
        name = name_;
    }

    uint64_t State::get_id() const {
        return id_;
    }

    void State::register_enter_callback(std::function<void()> func) {
        enter_fun_cb_ = std::move(func);
    }

    void State::register_exit_callback(std::function<void()> func) {
        exit_fun_cb_ = std::move(func);
    }

    void State::register_run_callback(std::function<void()> func) {
        run_fun_cb_ = std::move(func);
    }

    void State::register_events(Event* event) {
        v_event_ptr_.push_back(event);
    }

    void State::reset_event() {
        event_count_ = UINT64_MAX;
        if(!v_event_ptr_.empty()) {
            for(auto & it : v_event_ptr_) {
                it->reset_event_number();
            }
        }
    }

    std::vector<Event*> State::get_register_event() {
        return v_event_ptr_;
    }

    void State::translate(Event *event) {
        // 如果是的信号的话需要做处理
        if(!v_event_ptr_.empty()) {
            for(auto & it : v_event_ptr_) {
                if(it->get_id() == event->get_id()) {
                    if(it->get_event_number() == UINT64_MAX) {
                        event_count_++;
                        it->set_event_number(event_count_);
                    }
                    break;
                }
            }
        }
    }

    State* State::run() {
        State* state = this;

        if(is_first_run_enter_cb_) {
            if(enter_fun_cb_ != nullptr) {
                enter_fun_cb_();
            }
            is_first_run_enter_cb_ = false;
        }

        if(run_fun_cb_ != nullptr) {
            run_fun_cb_();
        }

        if(!v_event_ptr_.empty()) {
            for(auto & it : v_event_ptr_) {
                if(it->get_event_number() == 0) {
                    state = it->get_next_state();
                    is_run_exit_cb_ = true;
                    break;
                }
            }
        }

        if(exit_fun_cb_ != nullptr && is_run_exit_cb_) {
            is_run_exit_cb_ = false;
            is_first_run_enter_cb_ = true;
            exit_fun_cb_();
        }

        return state;
    }

    FSM::FSM() {
        current_state_ptr_ = nullptr;
        v_state_ptr_.resize(0);
    }

    FSM::FSM(State *init_state) {
        current_state_ptr_ = init_state;
        v_state_ptr_.resize(0);
        v_state_ptr_.push_back(init_state);
    }

    FSM::~FSM() {
        current_state_ptr_ = nullptr;
        v_state_ptr_.clear();
    }

    void FSM::add_init_state(State *state) {
        bool is_exit_flag = false;
        current_state_ptr_ = state;
        if(!v_state_ptr_.empty()) {
            for(auto & it : v_state_ptr_) {
                if(state->get_id() == it->get_id()) {
                    is_exit_flag = true;
                    break;
                }
            }
            if(!is_exit_flag) {
                std::cout << "add init state in fsm" << std::endl;
                v_state_ptr_.push_back(state);
            }
        }
        else {
            v_state_ptr_.push_back(state);
        }
    }

    void FSM::add_state(State* state) {
        bool is_exit_flag = false;
        if(!v_state_ptr_.empty()) {
            for(auto & it : v_state_ptr_) {
                if(state->get_id() == it->get_id()) {
                    is_exit_flag = true;
                    break;
                }
            }
            if(!is_exit_flag) {
                std::cout << "add state in fsm" << std::endl;
                v_state_ptr_.push_back(state);
            }
        }
        else {
            v_state_ptr_.push_back(state);
        }
    }

    void FSM::translate(Event *event) {
        mtx_.lock();
        current_state_ptr_->translate(event);
        mtx_.unlock();
    }

    bool FSM::start() {
        bool ret;
        mtx_.lock();
        if(current_state_ptr_ == nullptr) {
            ret = false;
        }
        else {
            current_state_ptr_ = current_state_ptr_->run();

            if(!v_state_ptr_.empty()) {
                for(auto & it : v_state_ptr_) {
                    if(it->get_id() != current_state_ptr_->get_id()) {
                        it->reset_event();
                    }
                }
            }

            ret = true;
        }
        mtx_.unlock();
        return ret;
    }
}
