#pragma once
#ifndef UVKE_FILE_HEADER
#define UVKE_FILE_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API File {
    public:
        File() = default;
        File(std::string_view filename);
        virtual ~File() = default;

        static std::span<char> Load(std::string_view filename);

        virtual std::span<char> GetData();

    private:
        std::vector<char> m_data;

    };
};

#endif