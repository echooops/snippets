#ifndef AUDIO_ENDPOINT_H
#define AUDIO_ENDPOINT_H

#include <string>
#include <unordered_map>

#include <Windows.h>
#include <winnt.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>


class DECLSPEC_UUID("f8679f50-850a-41cf-9c72-430f290290c8") IPolicyConfig;
class DECLSPEC_UUID("870af99c-171d-4f9e-af0d-e63df40c2bc9") CPolicyConfigClient;

class IPolicyConfig : public IUnknown {
public:

    virtual HRESULT STDMETHODCALLTYPE GetMixFormat(__in PCWSTR, __out WAVEFORMATEX **);

    virtual HRESULT STDMETHODCALLTYPE GetDeviceFormat(__in PCWSTR, __in INT, __out WAVEFORMATEX **);

    virtual HRESULT STDMETHODCALLTYPE ResetDeviceFormat(__in PCWSTR);

    virtual HRESULT STDMETHODCALLTYPE SetDeviceFormat(__in PCWSTR, __in WAVEFORMATEX *, __in WAVEFORMATEX *);

    virtual HRESULT STDMETHODCALLTYPE GetProcessingPeriod(__in PCWSTR, __in INT, __out PINT64, __out PINT64);

    virtual HRESULT STDMETHODCALLTYPE SetProcessingPeriod(__in PCWSTR, __in PINT64);

    virtual HRESULT STDMETHODCALLTYPE GetShareMode(__in PCWSTR, __out struct DeviceShareMode *);

    virtual HRESULT STDMETHODCALLTYPE SetShareMode(__in PCWSTR, __in struct DeviceShareMode *);

    virtual HRESULT STDMETHODCALLTYPE GetPropertyValue(__in PCWSTR, __in const PROPERTYKEY &, __out PROPVARIANT *);

    virtual HRESULT STDMETHODCALLTYPE SetPropertyValue(__in PCWSTR, __in const PROPERTYKEY &, __in PROPVARIANT *);

    virtual HRESULT STDMETHODCALLTYPE SetDefaultEndpoint(__in PCWSTR, __in ERole);

    virtual HRESULT STDMETHODCALLTYPE SetEndpointVisibility(__in PCWSTR, __in INT);
};

class AudioEndpoint {
    static std::unordered_map<std::wstring, std::wstring> EnumAudioEndpointsFriendlyName(IMMDeviceCollection *collection)
    {
        std::unordered_map<std::wstring, std::wstring> result;
        UINT count;
        HRESULT hr = collection->GetCount(&count);
        if (FAILED(hr)) {
            return result;
        }
        for (UINT i = 0; i < count; i++) {
            IMMDevice *device = nullptr;
            hr = collection->Item(i, &device);
            if (FAILED(hr) || !device) {
                continue;
            }
            LPWSTR id = nullptr;
            hr = device->GetId(&id);
            if (FAILED(hr)) {
                device->Release();
                continue;
            }
            IPropertyStore *store = nullptr;
            hr = device->OpenPropertyStore(STGM_READ, &store);
            if (SUCCEEDED(hr)) {
                PROPVARIANT friendlyName;
                PropVariantInit(&friendlyName);
                hr = store->GetValue(PKEY_Device_FriendlyName, &friendlyName);
                if (SUCCEEDED(hr)) {
                    result[id] = std::wstring(friendlyName.pwszVal);
                }
                PropVariantClear(&friendlyName);
                store->Release();
            }
            CoTaskMemFree(id);
            device->Release();
        }
        return result;
    }
public:
    static std::unordered_map<std::wstring, std::wstring> AudioEndpoint::EnumAudioEndpoints(bool isCapture)
    {
        std::unordered_map<std::wstring, std::wstring> result;
        IMMDeviceEnumerator *enumerator = nullptr;
        IMMDeviceCollection *collection = nullptr;
        HRESULT hr = CoInitialize(nullptr);
        bool isCoInitialized = true;
        if (FAILED(hr)) {
            isCoInitialized = false;
        }
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
                              __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&enumerator));
        if (SUCCEEDED(hr) && enumerator) {
            hr = enumerator->EnumAudioEndpoints(isCapture ? eCapture : eRender,
                                                DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED, &collection);
            if (SUCCEEDED(hr) && collection) {
                result = EnumAudioEndpointsFriendlyName(collection);
                collection->Release();
            }
            enumerator->Release();
        }
        if (isCoInitialized) {
            CoUninitialize();
        }
        return result;
    }
    /* 设置设备是否可见 */
    static int SetVisibility(std::wstring devicedId, bool isVisible)
    {
        HRESULT hr = CoInitialize(nullptr);
        bool isCoInitialized = true;
        if (FAILED(hr)) {
            isCoInitialized = false;
        }
        IPolicyConfig *policyConfig = nullptr;
        hr = CoCreateInstance(__uuidof(CPolicyConfigClient), nullptr, CLSCTX_ALL,
                              __uuidof(IPolicyConfig), (LPVOID *)&policyConfig);
        if (SUCCEEDED(hr)) {
            hr = policyConfig->SetEndpointVisibility((PCWSTR)devicedId.data(), isVisible);
            policyConfig->Release();
        }
        if (isCoInitialized) {
            CoUninitialize();
        }
        if (SUCCEEDED(hr)) {
            return 0;
        }
        return -1;
    }
    /* 设置默认设备 */
    static int SetDefault(std::wstring devicedId, ERole role)
    {
        HRESULT hr = CoInitialize(nullptr);
        bool isCoInitialized = true;
        if (FAILED(hr)) {
            isCoInitialized = false;
        }
        IPolicyConfig *policyConfig = nullptr;
        hr = CoCreateInstance(__uuidof(CPolicyConfigClient), nullptr, CLSCTX_ALL,
                              __uuidof(IPolicyConfig), (LPVOID *)&policyConfig);
        if (SUCCEEDED(hr)) {
            hr = policyConfig->SetDefaultEndpoint((PCWSTR)devicedId.data(), role);
            policyConfig->Release();
        }
        if (isCoInitialized) {
            CoUninitialize();
        }
        if (SUCCEEDED(hr)) {
            return 0;
        }
        return -1;
    }
    /* 获取默认扬声器设备 */
    static std::wstring GetDefaultSpeaker(ERole role)
    {
        std::wstring reslut;
        IMMDeviceEnumerator *enumerator = nullptr;
        HRESULT hr = CoInitialize(nullptr);
        bool isCoInitialized = true;
        if (FAILED(hr)) {
            isCoInitialized = false;
        }
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
                              __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&enumerator));
        if (SUCCEEDED(hr)) {
            IMMDevice *device = nullptr;
            hr = enumerator->GetDefaultAudioEndpoint(eRender, role, &device);
            if (SUCCEEDED(hr)) {
                LPWSTR id = nullptr;
                hr = device->GetId(&id);
                if (SUCCEEDED(hr) && id) {
                    reslut = id;
                    CoTaskMemFree(id);
                }
                device->Release();
            }
            enumerator->Release();
        }
        if (isCoInitialized) {
            CoUninitialize();
        }
        return reslut;
    }
};

#endif
