// mtar_wrap.hpp --- microtar wrapper class
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.
#ifndef MTAR_WRAP_HPP_
#define MTAR_WRAP_HPP_      1   // Version 1

#include "microtar.h"
#include <cstring>
#include <cassert>

typedef int mtar_err_t;     // MTAR_E...

class mtar_wrap
{
public:
    mtar_wrap();
    virtual ~mtar_wrap();
    mtar_err_t open(const char *filename, const char *mode);
    mtar_err_t open(const wchar_t *filename, const wchar_t *mode);
    mtar_err_t open_fp(void *fp);
    mtar_err_t open_memory(void *data, size_t size);
    bool is_open() const;
    mtar_err_t close();

    mtar_err_t find(const char *name, mtar_header_t *h);
    mtar_err_t seek(size_t pos);
    mtar_err_t rewind();
    mtar_err_t next();

    mtar_err_t read_header(mtar_header_t *h);
    mtar_err_t read_data(void *ptr, size_t size);

    mtar_err_t write_header(const mtar_header_t *h);
    mtar_err_t write_file_header(const char *name, size_t size);
    mtar_err_t write_dir_header(const char *name);
    mtar_err_t write_data(const void *data, size_t size);
    mtar_err_t finalize();

protected:
    mtar_t m_tar;

private:
    mtar_wrap(const mtar_wrap&);
    mtar_wrap& operator=(const mtar_wrap&);
};

//////////////////////////////////////////////////////////////////////////////

inline mtar_wrap::mtar_wrap()
{
    memset(&m_tar, 0, sizeof(m_tar));
}

inline mtar_wrap::~mtar_wrap()
{
    close();
}

inline mtar_err_t mtar_wrap::open(const char *filename, const char *mode)
{
    close();
    mtar_err_t ret = mtar_open(&m_tar, filename, mode);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::open(const wchar_t *filename, const wchar_t *mode)
{
    close();
    mtar_err_t ret = mtar_open_w(&m_tar, filename, mode);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::open_fp(void *fp)
{
    close();
    mtar_err_t ret = mtar_open_fp(&m_tar, fp);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::open_memory(void *data, size_t size)
{
    close();
    mtar_err_t ret = mtar_open_memory(&m_tar, data, size);
    assert(ret == 0);
    return ret;
}

inline bool mtar_wrap::is_open() const
{
    return m_tar.read || m_tar.write;
}

inline mtar_err_t mtar_wrap::close()
{
    mtar_err_t ret = MTAR_EFAILURE;
    if (is_open())
    {
        ret = mtar_close(&m_tar);
        memset(&m_tar, 0, sizeof(m_tar));
    }
    return ret;
}

inline mtar_err_t mtar_wrap::find(const char *name, mtar_header_t *h)
{
    assert(is_open());
    mtar_err_t ret = mtar_find(&m_tar, name, h);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::seek(size_t pos)
{
    assert(is_open());
    mtar_err_t ret = mtar_seek(&m_tar, pos);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::rewind()
{
    assert(is_open());
    mtar_err_t ret = mtar_rewind(&m_tar);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::next()
{
    assert(is_open());
    mtar_err_t ret = mtar_next(&m_tar);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::read_header(mtar_header_t *h)
{
    assert(is_open());
    mtar_err_t ret = mtar_read_header(&m_tar, h);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::read_data(void *ptr, size_t size)
{
    assert(is_open());
    mtar_err_t ret = mtar_read_data(&m_tar, ptr, size);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::write_header(const mtar_header_t *h)
{
    assert(is_open());
    mtar_err_t ret = mtar_write_header(&m_tar, h);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::write_file_header(const char *name, size_t size)
{
    assert(is_open());
    mtar_err_t ret = mtar_write_file_header(&m_tar, name, size);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::write_dir_header(const char *name)
{
    assert(is_open());
    mtar_err_t ret = mtar_write_dir_header(&m_tar, name);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::write_data(const void *data, size_t size)
{
    assert(is_open());
    mtar_err_t ret = mtar_write_data(&m_tar, data, size);
    assert(ret == 0);
    return ret;
}

inline mtar_err_t mtar_wrap::finalize()
{
    assert(is_open());
    mtar_err_t ret = mtar_finalize(&m_tar);
    assert(ret == 0);
    return ret;
}

#endif  // ndef MTAR_WRAP_HPP_
