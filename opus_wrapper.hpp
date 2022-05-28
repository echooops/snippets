#include <opus.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <atomic>

namespace opus {

struct CoderDestroyer {
    void operator()(OpusEncoder *encoder) const noexcept
    {
        opus_encoder_destroy(encoder);
    }
    void operator()(OpusDecoder *decoder) const noexcept
    {
        opus_decoder_destroy(decoder);
    }
};

/* 编码器封装 */
class Encoder final {
public:
    /*
    * sampleRate    : 采样率
    * channels      : 声道数
    * application   : 编码模式 OPUS_APPLICATION_VOIP、OPUS_APPLICATION_AUDIO、OPUS_APPLICATION_RESTRICTED_LOWDELAY
    *                 OPUS_APPLICATION_VOIP : 适合语音编码
    *                 OPUS_APPLICATION_AUDIO : 适合音乐编码
    *                 OPUS_APPLICATION_RESTRICTED_LOWDELAY : 适合低时延编码
    * losses        : 丢包率（非 0 FEC 为打开状态）
    */
    Encoder(opus_int32 sampleRate, int numChannels, int application, int losses = 0)
        : numChannels_(numChannels)
    {
        int error = OPUS_OK;
        encoder_.reset(opus_encoder_create(sampleRate, numChannels, application, &error));
        isValid_ = (error == OPUS_OK);
        if (!IsValid()) {
            return;
        }
        if (losses > 0) {
            Ctl(OPUS_SET_INBAND_FEC(1));
            Ctl(OPUS_SET_PACKET_LOSS_PERC(losses));
        }
    }
    bool IsValid() const
    {
        return isValid_;
    }
    /* 重置编码器状态，编码不同流时需要重置状态，解除编码上下文关联 */
    bool ResetState()
    {
        isValid_ = (Ctl(OPUS_RESET_STATE) == OPUS_OK);
        return isValid_;
    }
    /* 设置编码比特率，不进行设置则使用默认 */
    bool SetBitrate(int bitrate)
    {
        isValid_ = (Ctl(OPUS_SET_BITRATE(bitrate)) == OPUS_OK);
        return isValid_;
    }
    /* 启用或禁用可变比特率，默认启用 */
    bool SetVariableBitrate(int vbr)
    {
        isValid_ = (Ctl(OPUS_SET_VBR(vbr)) == OPUS_OK);
        return isValid_;
    }
    /* 设置编码复杂度 */
    bool SetComplexity(int complexity)
    {
        isValid_ = (Ctl(OPUS_SET_COMPLEXITY(complexity)) == OPUS_OK);
        return isValid_;
    }
    /* 编码一帧数据 */
    std::vector<unsigned char> EncodeFrame(const std::vector<opus_int16> &pcm, int frameSize)
    {
        std::vector<unsigned char> encoded(pcm.size() * sizeof(opus_int16));
        auto numBytes = opus_encode(encoder_.get(), pcm.data(), frameSize,
                                    encoded.data(), encoded.size());
        if (numBytes < 0) {
            return {};
        }
        encoded.resize(numBytes);
        return encoded;
    }
private:
    template <typename... Ts>
    int Ctl(int request, Ts... args) const
    {
        return opus_encoder_ctl(encoder_.get(), request, args...);
    }
private:
    int                                             numChannels_{ 0 };          // 声道数
    std::atomic_bool                                isValid_{ false };          // 是否有效
    std::unique_ptr<OpusEncoder, CoderDestroyer>    encoder_{ nullptr };        // 编码器句柄
};


/* 解码器封装 */
class Decoder final {
public:
    /*
    * sampleRate    : 采样率
    * channels      : 声道数
    */
    Decoder(opus_int32 sampleRate, int numChannels) : numChannels_(numChannels)
    {
        int error = OPUS_OK;
        decoder_.reset(opus_decoder_create(sampleRate, numChannels, &error));
        isValid_ = (error == OPUS_OK);
    }
    bool IsValid() const { return isValid_; }

    std::vector<opus_int16> DecodeFrame(std::vector<unsigned char> &packet, int frameSize, bool FEC = false)
    {
        std::vector<opus_int16> decoded(frameSize * numChannels_);
        auto numSamples = opus_decode(decoder_.get(), packet.data(), packet.size(),
                                      decoded.data(), frameSize, FEC);
        if (numSamples < 0) {
            return {};
        }
        decoded.resize(frameSize * numChannels_);
        return decoded;
    }

private:
    int                                             numChannels_{ 0 };          // 声道数
    std::atomic_bool                                isValid_{ false };          // 是否有效
    std::unique_ptr<OpusDecoder, CoderDestroyer>    decoder_{ nullptr };        // 解码器句柄
};

}
