#pragma once
#include "CPU.h"
#include "Emulate.h"

class SoftwareCPU : public CPU {
public:
    SoftwareCPU()
        : SoftwareCPU("default")
    {
    }
    SoftwareCPU(const std::string &name)
        : CPU(), emulator(&registers)
    {
        (void)name;

        emulator.set_memory(&mem);
        emulator.set_io(&io);
    }

    void write_reg(GPR regnum, uint16_t val)
    {
        registers.set(regnum, val);
    }

    uint16_t read_reg(GPR regnum)
    {
        return registers.get(regnum);
    }

    size_t step()
    {
        return emulator.emulate();
    }

    void write_flags(uint16_t val)
    {
        registers.set_flags(val);
    }

    uint16_t read_flags()
    {
        return registers.get_flags();
    }

    bool has_trapped()
    {
        return emulator.has_trapped();
    }

    void reset()
    {
        emulator.reset();
    }

    bool instruction_had_side_effects() const
    {
        return mem.has_written() || io.has_written() || registers.has_written();
    }

    void clear_side_effects()
    {
        registers.clear_has_written();
        mem.clear_has_written();
        io.clear_has_written();
    }

    void write_mem8(uint16_t segment, uint16_t addr, uint8_t val)
    {
        mem.write<uint8_t>(get_phys_addr(segment, addr), val);
    }
    void write_mem16(uint16_t segment, uint16_t addr, uint16_t val)
    {
        mem.write<uint16_t>(get_phys_addr(segment, addr), val);
    }
    void write_mem32(uint16_t segment, uint16_t addr, uint32_t val)
    {
        mem.write<uint32_t>(get_phys_addr(segment, addr), val);
    }

    uint8_t read_mem8(uint16_t segment, uint16_t addr)
    {
        return mem.read<uint8_t>(get_phys_addr(segment, addr));
    }
    uint16_t read_mem16(uint16_t segment, uint16_t addr)
    {
        return mem.read<uint16_t>(get_phys_addr(segment, addr));
    }
    uint32_t read_mem32(uint16_t segment, uint16_t addr)
    {
        return mem.read<uint32_t>(get_phys_addr(segment, addr));
    }

    void write_io8(uint32_t addr, uint8_t val)
    {
        io.write<uint8_t>(addr, val);
    }
    void write_io16(uint32_t addr, uint16_t val)
    {
        io.write<uint16_t>(addr, val);
    }

    uint8_t read_io8(uint32_t addr)
    {
        return io.read<uint8_t>(addr);
    }
    uint16_t read_io16(uint32_t addr)
    {
        return io.read<uint16_t>(addr);
    }

    virtual bool has_instruction_length() const
    {
        return true;
    }
private:
    RegisterFile registers;
    Emulator emulator;
    Memory mem;
    Memory io;
};
