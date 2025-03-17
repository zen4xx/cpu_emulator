#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread>

#define MOV 0x01
#define ADD 0x02
#define SUB 0x03
#define CMP 0x04
#define JE  0x05
#define JNE 0x06
#define JUMPTO 0x07

struct command{
    uint8_t opcode;
    short frst;
    short scnd;
};

class Cpu{

public:
    Cpu(){
        for(short i = 0; i < 16; ++i){
            registers[i] = 0;
        }
    }

    void mov(short reg, short val){
        commands.push_back((command){MOV, reg, val});
    }

    void add(short frst_reg, short scnd_reg){
        commands.push_back((command){ADD, frst_reg, scnd_reg});
    }

    void sub(short frst_reg, short scnd_reg){
        commands.push_back((command){SUB, frst_reg, scnd_reg});
    }

    void cmp(short reg, short val){
        commands.push_back((command){CMP, reg, val});
    }

    void je(short line){
        commands.push_back((command){JE, line, 0});
    }

    void jne(short line){
        commands.push_back((command){JNE, line, 0});
    }

    void jumpto(short line){
        commands.push_back((command){JUMPTO, line, 0});
    }

    // milliseconds
    void set_delay(int delay){
        m_delay = delay;
    }  

    void run(){
        command cmd;
        char is_translate = 0;
        for(int i = 0; i < commands.size(); ++i){
            std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
            cmd = commands[i];
            system("clear");
            print_regs();
            switch(cmd.opcode){
                case MOV:
                    registers[cmd.frst] = cmd.scnd;
                    break;

                case ADD:
                    registers[cmd.frst] += registers[cmd.scnd];
                    break;

                case SUB:
                    registers[cmd.frst] -= registers[cmd.scnd];
                    break;
                case CMP:
                    if(registers[cmd.frst] != cmd.scnd && i < commands.size() - 1){
                        is_translate = -1;
                    }
                    else{
                        is_translate = 1;
                    }
                    break;
                case JE:
                    if(is_translate == 1){
                        i = cmd.frst;
                        is_translate = 0;
                    }
                    break;
                case JNE:
                    if(is_translate == -1){
                        i = cmd.frst;
                        is_translate = 0;
                    }
                    break;
                case JUMPTO:
                    i = cmd.frst;
                    break;
            }
        }
    }

    void print_regs(){
        for(int i = 0; i < 16; ++i){
            std::cout << registers[i] << std::endl;
        }
    }

private:
    std::vector<command> commands;
    short registers[16];
    int m_delay = 0;
};

int main(){
    Cpu cpu;
    cpu.set_delay(500);
    char sign;
    int a,b;
    std::cin >> a;
    std::cin >> sign;
    std::cin >> b;
    cpu.mov(0, a);
    cpu.mov(1, b);
    cpu.mov(2, sign);
    cpu.cmp(2, '+');
    cpu.jne(8);
    cpu.add(0, 1);
    cpu.jumpto(9);
    cpu.sub(0, 1);
    cpu.run();
}