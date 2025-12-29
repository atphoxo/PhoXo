#include "pch.h"
#include "jp2.h"
#include <openjpeg.h>

namespace
{
    // 完全从OpenJpeg拷贝过来，为了让它支持unicode文件
    static OPJ_BOOL opj_seek_from_file(OPJ_OFF_T p_nb_bytes, void * p_user_data)
    {
        if (_fseeki64((FILE*)p_user_data, p_nb_bytes, SEEK_SET)) {
            return OPJ_FALSE;
        }
        return OPJ_TRUE;
    }

    static OPJ_OFF_T opj_skip_from_file(OPJ_OFF_T p_nb_bytes, void * p_user_data)
    {
        if (_fseeki64((FILE*)p_user_data, p_nb_bytes, SEEK_CUR)) {
            return -1;
        }
        return p_nb_bytes;
    }

    static OPJ_SIZE_T opj_write_from_file(void * p_buffer, OPJ_SIZE_T p_nb_bytes, void * p_user_data)
    {
        return fwrite(p_buffer, 1, p_nb_bytes, (FILE*)p_user_data);
    }

    static OPJ_SIZE_T opj_read_from_file(void * p_buffer, OPJ_SIZE_T p_nb_bytes, void * p_user_data)
    {
        OPJ_SIZE_T l_nb_read = fread(p_buffer, 1, p_nb_bytes, (FILE*)p_user_data);
        return l_nb_read ? l_nb_read : (OPJ_SIZE_T)-1;
    }

    static OPJ_UINT64 opj_get_data_length_from_file(void * p_user_data)
    {
        FILE* p_file = (FILE*)p_user_data;

        _fseeki64(p_file, 0, SEEK_END);
        OPJ_OFF_T file_length = (OPJ_OFF_T)_ftelli64(p_file);
        _fseeki64(p_file, 0, SEEK_SET);

        return (OPJ_UINT64)file_length;
    }

    static void opj_close_from_file(void* p_user_data)
    {
        fclose((FILE*)p_user_data);
    }

    opj_stream_t* OPJ_CALLCONV opj_stream_create_file_streamW(
        const wchar_t *fname,
        OPJ_SIZE_T p_size,
        OPJ_BOOL p_is_read_stream)
    {
        opj_stream_t* l_stream{};
        FILE *p_file{};
        const wchar_t *mode{};

        if (!fname) {
            return NULL;
        }

        if (p_is_read_stream) {
            mode = L"rb";
        }
        else {
            mode = L"wb";
        }

        _wfopen_s(&p_file, fname, mode);

        if (!p_file) {
            return NULL;
        }

        l_stream = opj_stream_create(p_size, p_is_read_stream);
        if (!l_stream) {
            fclose(p_file);
            return NULL;
        }

        opj_stream_set_user_data(l_stream, p_file, opj_close_from_file);
        opj_stream_set_user_data_length(l_stream,
            opj_get_data_length_from_file(p_file));
        opj_stream_set_read_function(l_stream, opj_read_from_file);
        opj_stream_set_write_function(l_stream,
            (opj_stream_write_fn)opj_write_from_file);
        opj_stream_set_skip_function(l_stream, opj_skip_from_file);
        opj_stream_set_seek_function(l_stream, opj_seek_from_file);

        return l_stream;
    }

    //---------------------------------------------------------------------------------------------
    OPJ_CODEC_FORMAT GetTypeByName(PCWSTR filepath)
    {
        CString   ext = PathFindExtension(filepath);
        ext.MakeLower();
        if (ext == L".jp2") return OPJ_CODEC_JP2;
        if (ext == L".j2k") return OPJ_CODEC_J2K;
        return OPJ_CODEC_UNKNOWN;
    }

    struct OpjCodec
    {
        opj_codec_t   * m_p{};
        opj_dparameters_t   m_param;

        OpjCodec(PCWSTR filepath) : m_p(opj_create_decompress(GetTypeByName(filepath)))
        {
            opj_set_default_decoder_parameters(&m_param);
            opj_setup_decoder(m_p, &m_param);
        }

        ~OpjCodec() { opj_destroy_codec(m_p); }
        operator opj_codec_t*() const { return m_p; }
    };

    struct OpjStream
    {
        opj_stream_t   * m_p{};

        OpjStream(PCWSTR filepath) : m_p(opj_stream_create_file_streamW(filepath, OPJ_J2K_STREAM_CHUNK_SIZE, OPJ_TRUE)) {}
        ~OpjStream() { opj_stream_destroy(m_p); }
        operator opj_stream_t*() const { return m_p; }
    };

    struct OpjImage
    {
        opj_image_t   * m_p{};
        IWICBitmapPtr   m_bmp;

        OpjImage(opj_stream_t* stream, opj_codec_t* codec, IWICImagingFactory* factory)
        {
            if (opj_read_header(stream, codec, &m_p) && m_p)
            {
                if (opj_decode(codec, stream, m_p) && IsValid())
                {
                    factory->CreateBitmap(m_p->x1 - m_p->x0, m_p->y1 - m_p->y0, WICNormal32bpp, WICBitmapCacheOnLoad, &m_bmp);
                    if (WIC::BitmapLock lock(m_bmp); lock.IsFormat32bpp())
                    {
                        CopyTo((FCColor*)lock.m_data, lock.m_width * lock.m_height);
                    }
                }
            }
        }

        ~OpjImage() { opj_image_destroy(m_p); }
        operator opj_image_t*() const { return m_p; }

        bool IsValid() const
        {
            return IsAllChannelsValid() && (m_p->numcomps >= 3 || m_p->numcomps == 1);
        }

        bool IsAllChannelsValid() const
        {
            for (UINT32 i = 0; i < m_p->numcomps; i++)
            {
                if (auto& cp = m_p->comps[i]; cp.dx != 1 || cp.dy != 1 || (cp.prec < 8) || (cp.prec > 16))
                    return false;
            }
            return true;
        }

        BYTE Clamp8(int v) const
        {
            int   prec = m_p->comps[0].prec;
            if (prec == 8)
                return (BYTE)std::clamp(v, 0, 0xFF);

            int   maxval = (1 << prec) - 1;
            v = std::clamp(v, 0, maxval);
            return (BYTE)(v * 255 / maxval);
        }

        void CopyTo(FCColor* dest, UINT total) const
        {
            if (m_p->color_space == OPJ_CLRSPC_SYCC && m_p->numcomps >= 3)
            {
                int*   Y = m_p->comps[0].data;
                int*   Cb = m_p->comps[1].data;
                int*   Cr = m_p->comps[2].data;
                for (UINT i = 0; i < total; i++, dest++, Y++, Cb++, Cr++)
                {
                    int   r = (int)(*Y + 1.402 * (*Cr - 128));
                    int   g = (int)(*Y - 0.344136 * (*Cb - 128) - 0.714136 * (*Cr - 128));
                    int   b = (int)(*Y + 1.772 * (*Cb - 128));
                    *dest = { Clamp8(b), Clamp8(g), Clamp8(r), 0xFF };
                }
            }
            else
            {
                int*   r = m_p->comps[0].data;
                int*   g = (m_p->numcomps == 1) ? r : m_p->comps[1].data;
                int*   b = (m_p->numcomps == 1) ? r : m_p->comps[2].data;
                for (UINT i = 0; i < total; i++, dest++, r++, g++, b++)
                {
                    *dest = { Clamp8(*b), Clamp8(*g), Clamp8(*r), 0xFF };
                }
            }
        }
    };
}

IWICBitmap* oxo_jpeg2000_load(PCWSTR filepath, IWICImagingFactory* factory)
{
    if (OpjCodec codec(filepath); codec)
    {
        if (OpjStream stream(filepath); stream)
        {
            OpjImage   image(stream, codec, factory);
            opj_end_decompress(codec, stream);
            return image.m_bmp.Detach();
        }
    }
    return nullptr;
}
