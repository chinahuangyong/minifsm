#include <iostream>

#include <fsm.h>
#include <thread>

minifsm::State s1;
minifsm::Event ev1;
minifsm::State s2;
minifsm::Event ev2;
minifsm::State s3;
minifsm::Event ev3;

minifsm::FSM fsm;

void s1_enter_callback(void) {
    std::cout << "s1 enter" << std::endl;
}

void s1_run_callback(void) {
//    std::cout << "s1 run" << std::endl;
}

void s1_exit_callback(void) {
    std::cout << "s1 exit" << std::endl;
}

void s2_enter_callback(void) {
    std::cout << "s2 enter" << std::endl;
}

void s2_run_callback(void) {
//    std::cout << "s2 run" << std::endl;
}


void s2_exit_callback(void) {
    std::cout << "s2 exit" << std::endl;
}

void s3_enter_callback(void) {
    std::cout << "s3 enter" << std::endl;
}

void s3_run_callback(void) {
//    std::cout << "s3run" << std::endl;
}


void s3_exit_callback(void) {
    std::cout << "s3 exit" << std::endl;
}

void my_thread(void) {
    char a;
    while(1) {
        std::cin >> a;

        if(a == '1') {
            fsm.translate(&ev1);
//            std::cout << "ev1 is set" << std::endl;
        }
        else if(a == '2') {
            fsm.translate(&ev2);
//            std::cout << "ev2 is set" << std::endl;
        }
        else if(a == '3') {
            fsm.translate(&ev3);
//            std::cout << "ev3 is set" << std::endl;
        }
    }
}

int main() {
    s1.register_enter_callback(s1_enter_callback);
    s1.register_exit_callback(s1_exit_callback);
    s1.register_run_callback(s1_run_callback);

    s2.register_enter_callback(s2_enter_callback);
    s2.register_exit_callback(s2_exit_callback);
    s2.register_run_callback(s2_run_callback);

    s3.register_enter_callback(s3_enter_callback);
    s3.register_exit_callback(s3_exit_callback);
    s3.register_run_callback(s3_run_callback);

    s1.register_events(&ev1);
    ev1.register_next_state(&s2);
    s2.register_events(&ev2);
    ev2.register_next_state(&s1);
    // s3.register_events(&ev3);
    // ev3.register_next_state(&s1);


    fsm.add_init_state(&s1);
    fsm.add_state(&s2);
    fsm.add_state(&s3);

    std::thread t1(my_thread);

    for(;;) {
        fsm.start();
    }
}
