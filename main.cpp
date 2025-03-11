#include <cstdint>
#include <iostream>
#include <vector>

#define MOV 0x01
#define ADD 0x02
#define SUB 0x03
#define CMP 0x04
#define JE  0x05
#define JNE 0x06

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

    void run(){
        command cmd;
        char is_translate = 0;
        for(int i = 0; i < commands.size(); ++i){
            cmd = commands[i];
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
                case JNE:
                    if(is_translate == -1){
                        i = cmd.frst;
                        is_translate = 0;
                    }
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
};

int main(){
    Cpu cpu;
    cpu.mov(0, 4);
    cpu.mov(1, 2);
    cpu.add(0,1);
    cpu.mov(6, 3);
    cpu.mov(3, 1);
    cpu.sub(0, 6);
    cpu.add(0, 3);
    cpu.cmp(0, 40);
    cpu.jne(5);
    cpu.run();
    cpu.print_regs();
}