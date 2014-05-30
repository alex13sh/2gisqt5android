/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtMultimedia/qmediametadata.h>
#include <QtCore/qcoreapplication.h>
#include <QSize>
#include <qdatetime.h>
#include <qimage.h>

#include <dshow.h>
#include <initguid.h>
#include <qnetwork.h>

#include "directshowmetadatacontrol.h"
#include "directshowplayerservice.h"

#ifndef QT_NO_WMSDK
#include <wmsdk.h>
#endif

#ifndef QT_NO_SHELLITEM
#include <ShlObj.h>
#include <propkeydef.h>
#include <private/qsystemlibrary_p.h>

DEFINE_PROPERTYKEY(PKEY_Author, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9, 4);
DEFINE_PROPERTYKEY(PKEY_Title, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9, 2);
DEFINE_PROPERTYKEY(PKEY_Media_SubTitle, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 38);
DEFINE_PROPERTYKEY(PKEY_ParentalRating, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 21);
DEFINE_PROPERTYKEY(PKEY_Comment, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9, 6);
DEFINE_PROPERTYKEY(PKEY_Copyright, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 11);
DEFINE_PROPERTYKEY(PKEY_Media_ProviderStyle, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 40);
DEFINE_PROPERTYKEY(PKEY_Media_Year, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 5);
DEFINE_PROPERTYKEY(PKEY_Media_DateEncoded, 0x2E4B640D, 0x5019, 0x46D8, 0x88, 0x81, 0x55, 0x41, 0x4C, 0xC5, 0xCA, 0xA0, 100);
DEFINE_PROPERTYKEY(PKEY_Rating, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 9);
DEFINE_PROPERTYKEY(PKEY_Keywords, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9, 5);
DEFINE_PROPERTYKEY(PKEY_Language, 0xD5CDD502, 0x2E9C, 0x101B, 0x93, 0x97, 0x08, 0x00, 0x2B, 0x2C, 0xF9, 0xAE, 28);
DEFINE_PROPERTYKEY(PKEY_Media_Publisher, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 30);
DEFINE_PROPERTYKEY(PKEY_Media_Duration, 0x64440490, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 3);
DEFINE_PROPERTYKEY(PKEY_Audio_EncodingBitrate, 0x64440490, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 4);
DEFINE_PROPERTYKEY(PKEY_Media_AverageLevel, 0x09EDD5B6, 0xB301, 0x43C5, 0x99, 0x90, 0xD0, 0x03, 0x02, 0xEF, 0xFD, 0x46, 100);
DEFINE_PROPERTYKEY(PKEY_Audio_ChannelCount, 0x64440490, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 7);
DEFINE_PROPERTYKEY(PKEY_Audio_PeakValue, 0x2579E5D0, 0x1116, 0x4084, 0xBD, 0x9A, 0x9B, 0x4F, 0x7C, 0xB4, 0xDF, 0x5E, 100);
DEFINE_PROPERTYKEY(PKEY_Audio_SampleRate, 0x64440490, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 5);
DEFINE_PROPERTYKEY(PKEY_Music_AlbumTitle, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 4);
DEFINE_PROPERTYKEY(PKEY_Music_AlbumArtist, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 13);
DEFINE_PROPERTYKEY(PKEY_Music_Artist, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 2);
DEFINE_PROPERTYKEY(PKEY_Music_Composer, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 19);
DEFINE_PROPERTYKEY(PKEY_Music_Conductor, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 36);
DEFINE_PROPERTYKEY(PKEY_Music_Lyrics, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 12);
DEFINE_PROPERTYKEY(PKEY_Music_Mood, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 39);
DEFINE_PROPERTYKEY(PKEY_Music_TrackNumber, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 7);
DEFINE_PROPERTYKEY(PKEY_Music_Genre, 0x56A3372E, 0xCE9C, 0x11D2, 0x9F, 0x0E, 0x00, 0x60, 0x97, 0xC6, 0x86, 0xF6, 11);
DEFINE_PROPERTYKEY(PKEY_ThumbnailStream, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9, 27);
DEFINE_PROPERTYKEY(PKEY_Video_FrameHeight, 0x64440491, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 4);
DEFINE_PROPERTYKEY(PKEY_Video_FrameWidth, 0x64440491, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 3);
DEFINE_PROPERTYKEY(PKEY_Video_HorizontalAspectRatio, 0x64440491, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 42);
DEFINE_PROPERTYKEY(PKEY_Video_VerticalAspectRatio, 0x64440491, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 45);
DEFINE_PROPERTYKEY(PKEY_Video_FrameRate, 0x64440491, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 6);
DEFINE_PROPERTYKEY(PKEY_Video_EncodingBitrate, 0x64440491, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 8);
DEFINE_PROPERTYKEY(PKEY_Video_Director, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 20);
DEFINE_PROPERTYKEY(PKEY_Media_Writer, 0x64440492, 0x4C8B, 0x11D1, 0x8B, 0x70, 0x08, 0x00, 0x36, 0xB1, 0x1A, 0x03, 23);

typedef HRESULT (WINAPI *q_SHCreateItemFromParsingName)(PCWSTR, IBindCtx *, const GUID&, void **);
static q_SHCreateItemFromParsingName sHCreateItemFromParsingName = 0;
#endif

#ifndef QT_NO_WMSDK
namespace
{
    struct QWMMetaDataKeyLookup
    {
        QString key;
        const wchar_t *token;
    };
}

static const QWMMetaDataKeyLookup qt_wmMetaDataKeys[] =
{
    { QMediaMetaData::Title, L"Title" },
    { QMediaMetaData::SubTitle, L"WM/SubTitle" },
    { QMediaMetaData::Author, L"Author" },
    { QMediaMetaData::Comment, L"Comment" },
    { QMediaMetaData::Description, L"Description" },
    { QMediaMetaData::Category, L"WM/Category" },
    { QMediaMetaData::Genre, L"WM/Genre" },
    //{ QMediaMetaData::Date, 0 },
    { QMediaMetaData::Year, L"WM/Year" },
    { QMediaMetaData::UserRating, L"Rating" },
    //{ QMediaMetaData::MetaDatawords, 0 },
    { QMediaMetaData::Language, L"WM/Language" },
    { QMediaMetaData::Publisher, L"WM/Publisher" },
    { QMediaMetaData::Copyright, L"Copyright" },
    { QMediaMetaData::ParentalRating, L"WM/ParentalRating" },
    //{ QMediaMetaData::RatingOrganisation, L"RatingOrganisation" },

    // Media
    { QMediaMetaData::Size, L"FileSize" },
    { QMediaMetaData::MediaType, L"MediaType" },
    { QMediaMetaData::Duration, L"Duration" },

    // Audio
    { QMediaMetaData::AudioBitRate, L"AudioBitRate" },
    { QMediaMetaData::AudioCodec, L"AudioCodec" },
    { QMediaMetaData::ChannelCount, L"ChannelCount" },
    { QMediaMetaData::SampleRate, L"Frequency" },

    // Music
    { QMediaMetaData::AlbumTitle, L"WM/AlbumTitle" },
    { QMediaMetaData::AlbumArtist, L"WM/AlbumArtist" },
    { QMediaMetaData::ContributingArtist, L"Author" },
    { QMediaMetaData::Composer, L"WM/Composer" },
    { QMediaMetaData::Conductor, L"WM/Conductor" },
    { QMediaMetaData::Lyrics, L"WM/Lyrics" },
    { QMediaMetaData::Mood, L"WM/Mood" },
    { QMediaMetaData::TrackNumber, L"WM/TrackNumber" },
    //{ QMediaMetaData::TrackCount, 0 },
    //{ QMediaMetaData::CoverArtUriSmall, 0 },
    //{ QMediaMetaData::CoverArtUriLarge, 0 },

    // Image/Video
    { QMediaMetaData::Resolution, L"WM/VideoHeight" },
    { QMediaMetaData::PixelAspectRatio, L"AspectRatioX" },

    // Video
    { QMediaMetaData::VideoFrameRate, L"WM/VideoFrameRate" },
    { QMediaMetaData::VideoBitRate, L"VideoBitRate" },
    { QMediaMetaData::VideoCodec, L"VideoCodec" },

    //{ QMediaMetaData::PosterUri, 0 },

    // Movie
    { QMediaMetaData::ChapterNumber, L"ChapterNumber" },
    { QMediaMetaData::Director, L"WM/Director" },
    { QMediaMetaData::LeadPerformer, L"LeadPerformer" },
    { QMediaMetaData::Writer, L"WM/Writer" },
};

static QVariant getValue(IWMHeaderInfo *header, const wchar_t *key)
{
    WORD streamNumber = 0;
    WMT_ATTR_DATATYPE type = WMT_TYPE_DWORD;
    WORD size = 0;

    if (header->GetAttributeByName(&streamNumber, key, &type, 0, &size) == S_OK) {
        switch (type) {
        case WMT_TYPE_DWORD:
            if (size == sizeof(DWORD)) {
                DWORD word;
                if (header->GetAttributeByName(
                        &streamNumber,
                        key,
                        &type,
                        reinterpret_cast<BYTE *>(&word),
                        &size) == S_OK) {
                    return int(word);
                }
            }
            break;
        case WMT_TYPE_STRING:
            {
                QString string;
                string.resize(size / 2); // size is in bytes, string is in UTF16

                if (header->GetAttributeByName(
                        &streamNumber,
                        key,
                        &type,
                        reinterpret_cast<BYTE *>(const_cast<ushort *>(string.utf16())),
                        &size) == S_OK) {
                    return string;
                }
            }
            break;
        case WMT_TYPE_BINARY:
            {
                QByteArray bytes;
                bytes.resize(size);
                if (header->GetAttributeByName(
                        &streamNumber,
                        key,
                        &type,
                        reinterpret_cast<BYTE *>(bytes.data()),
                        &size) == S_OK) {
                    return bytes;
                }
            }
            break;
        case WMT_TYPE_BOOL:
            if (size == sizeof(DWORD)) {
                DWORD word;
                if (header->GetAttributeByName(
                        &streamNumber,
                        key,
                        &type,
                        reinterpret_cast<BYTE *>(&word),
                        &size) == S_OK) {
                    return bool(word);
                }
            }
            break;
        case WMT_TYPE_QWORD:
            if (size == sizeof(QWORD)) {
                QWORD word;
                if (header->GetAttributeByName(
                        &streamNumber,
                        key,
                        &type,
                        reinterpret_cast<BYTE *>(&word),
                        &size) == S_OK) {
                    return qint64(word);
                }
            }
            break;
        case WMT_TYPE_WORD:
            if (size == sizeof(WORD)){
                WORD word;
                if (header->GetAttributeByName(
                        &streamNumber,
                        key,
                        &type,
                        reinterpret_cast<BYTE *>(&word),
                        &size) == S_OK) {
                    return short(word);
                }
            }
            break;
        case WMT_TYPE_GUID:
            if (size == 16) {
            }
            break;
        default:
            break;
        }
    }
    return QVariant();
}
#endif

#ifndef QT_NO_SHELLITEM
static QVariant convertValue(const PROPVARIANT& var)
{
    QVariant value;
    switch (var.vt) {
    case VT_LPWSTR:
        value = QString::fromUtf16(reinterpret_cast<const ushort*>(var.pwszVal));
        break;
    case VT_UI4:
        value = uint(var.ulVal);
        break;
    case VT_UI8:
        value = qulonglong(var.uhVal.QuadPart);
        break;
    case VT_BOOL:
        value = bool(var.boolVal);
        break;
    case VT_FILETIME:
        SYSTEMTIME sysDate;
        if (!FileTimeToSystemTime(&var.filetime, &sysDate))
            break;
        value = QDate(sysDate.wYear, sysDate.wMonth, sysDate.wDay);
        break;
    case VT_STREAM:
    {
        STATSTG stat;
        if (FAILED(var.pStream->Stat(&stat, STATFLAG_NONAME)))
            break;
        void *data = malloc(stat.cbSize.QuadPart);
        ULONG read = 0;
        if (FAILED(var.pStream->Read(data, stat.cbSize.QuadPart, &read))) {
            free(data);
            break;
        }
        value = QImage::fromData(reinterpret_cast<const uchar*>(data), read);
        free(data);
    }
        break;
    case VT_VECTOR | VT_LPWSTR:
        QStringList vList;
        for (ULONG i = 0; i < var.calpwstr.cElems; ++i)
            vList.append(QString::fromUtf16(reinterpret_cast<const ushort*>(var.calpwstr.pElems[i])));
        value = vList;
        break;
    }
    return value;
}
#endif

DirectShowMetaDataControl::DirectShowMetaDataControl(QObject *parent)
    : QMetaDataReaderControl(parent)
    , m_available(false)
{
}

DirectShowMetaDataControl::~DirectShowMetaDataControl()
{
}

bool DirectShowMetaDataControl::isMetaDataAvailable() const
{
    return m_available;
}

QVariant DirectShowMetaDataControl::metaData(const QString &key) const
{
    return m_metadata.value(key);
}

QStringList DirectShowMetaDataControl::availableMetaData() const
{
    return m_metadata.keys();
}

static QString convertBSTR(BSTR *string)
{
    QString value = QString::fromUtf16(reinterpret_cast<ushort *>(*string),
                                       ::SysStringLen(*string));

    ::SysFreeString(*string);
    string = 0;

    return value;
}

void DirectShowMetaDataControl::updateGraph(IFilterGraph2 *graph, IBaseFilter *source, const QString &fileSrc)
{
    m_metadata.clear();

#ifndef QT_NO_SHELLITEM
    if (!sHCreateItemFromParsingName) {
        QSystemLibrary lib(QStringLiteral("shell32"));
        sHCreateItemFromParsingName = (q_SHCreateItemFromParsingName)(lib.resolve("SHCreateItemFromParsingName"));
    }

    if (!fileSrc.isEmpty() && sHCreateItemFromParsingName) {
        IShellItem2* shellItem = 0;
        if (sHCreateItemFromParsingName(reinterpret_cast<const WCHAR*>(fileSrc.utf16()),
                                        0, IID_PPV_ARGS(&shellItem)) == S_OK) {

            IPropertyStore *pStore = 0;
            if (shellItem->GetPropertyStore(GPS_DEFAULT, IID_PPV_ARGS(&pStore)) == S_OK) {
                DWORD cProps;
                if (SUCCEEDED(pStore->GetCount(&cProps))) {
                    for (DWORD i = 0; i < cProps; ++i)
                    {
                        PROPERTYKEY key;
                        PROPVARIANT var;
                        PropVariantInit(&var);
                        if (FAILED(pStore->GetAt(i, &key)))
                            continue;
                        if (FAILED(pStore->GetValue(key, &var)))
                            continue;

                        if (IsEqualPropertyKey(key, PKEY_Author)) {
                            m_metadata.insert(QMediaMetaData::Author, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Title)) {
                            m_metadata.insert(QMediaMetaData::Title, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_SubTitle)) {
                            m_metadata.insert(QMediaMetaData::SubTitle, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_ParentalRating)) {
                            m_metadata.insert(QMediaMetaData::ParentalRating, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Comment)) {
                            m_metadata.insert(QMediaMetaData::Description, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Copyright)) {
                            m_metadata.insert(QMediaMetaData::Copyright, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_ProviderStyle)) {
                            m_metadata.insert(QMediaMetaData::Genre, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_Year)) {
                            m_metadata.insert(QMediaMetaData::Year, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_DateEncoded)) {
                            m_metadata.insert(QMediaMetaData::Date, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Rating)) {
                            m_metadata.insert(QMediaMetaData::UserRating,
                                              int((convertValue(var).toUInt() - 1) / qreal(98) * 100));
                        } else if (IsEqualPropertyKey(key, PKEY_Keywords)) {
                            m_metadata.insert(QMediaMetaData::Keywords, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Language)) {
                            m_metadata.insert(QMediaMetaData::Language, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_Publisher)) {
                            m_metadata.insert(QMediaMetaData::Publisher, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_Duration)) {
                            m_metadata.insert(QMediaMetaData::Duration,
                                              (convertValue(var).toLongLong() + 10000) / 10000);
                        } else if (IsEqualPropertyKey(key, PKEY_Audio_EncodingBitrate)) {
                            m_metadata.insert(QMediaMetaData::AudioBitRate, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_AverageLevel)) {
                            m_metadata.insert(QMediaMetaData::AverageLevel, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Audio_ChannelCount)) {
                            m_metadata.insert(QMediaMetaData::ChannelCount, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Audio_PeakValue)) {
                            m_metadata.insert(QMediaMetaData::PeakValue, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Audio_SampleRate)) {
                            m_metadata.insert(QMediaMetaData::SampleRate, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_AlbumTitle)) {
                            m_metadata.insert(QMediaMetaData::AlbumTitle, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_AlbumArtist)) {
                            m_metadata.insert(QMediaMetaData::AlbumArtist, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_Artist)) {
                            m_metadata.insert(QMediaMetaData::ContributingArtist, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_Composer)) {
                            m_metadata.insert(QMediaMetaData::Composer, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_Conductor)) {
                            m_metadata.insert(QMediaMetaData::Conductor, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_Lyrics)) {
                            m_metadata.insert(QMediaMetaData::Lyrics, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_Mood)) {
                            m_metadata.insert(QMediaMetaData::Mood, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_TrackNumber)) {
                            m_metadata.insert(QMediaMetaData::TrackNumber, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Music_Genre)) {
                            m_metadata.insert(QMediaMetaData::Genre, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_ThumbnailStream)) {
                            m_metadata.insert(QMediaMetaData::ThumbnailImage, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Video_FrameHeight)) {
                            QSize res;
                            res.setHeight(convertValue(var).toUInt());
                            if (SUCCEEDED(pStore->GetValue(PKEY_Video_FrameWidth, &var)))
                                res.setWidth(convertValue(var).toUInt());
                            m_metadata.insert(QMediaMetaData::Resolution, res);
                        } else if (IsEqualPropertyKey(key, PKEY_Video_HorizontalAspectRatio)) {
                            QSize aspectRatio;
                            aspectRatio.setWidth(convertValue(var).toUInt());
                            if (SUCCEEDED(pStore->GetValue(PKEY_Video_VerticalAspectRatio, &var)))
                                aspectRatio.setHeight(convertValue(var).toUInt());
                            m_metadata.insert(QMediaMetaData::PixelAspectRatio, aspectRatio);
                        } else if (IsEqualPropertyKey(key, PKEY_Video_FrameRate)) {
                            m_metadata.insert(QMediaMetaData::VideoFrameRate,
                                              convertValue(var).toReal() / 1000);
                        } else if (IsEqualPropertyKey(key, PKEY_Video_EncodingBitrate)) {
                            m_metadata.insert(QMediaMetaData::VideoBitRate, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Video_Director)) {
                            m_metadata.insert(QMediaMetaData::Director, convertValue(var));
                        } else if (IsEqualPropertyKey(key, PKEY_Media_Writer)) {
                            m_metadata.insert(QMediaMetaData::Writer, convertValue(var));
                        }

                        PropVariantClear(&var);
                    }
                }

                pStore->Release();
            }

            shellItem->Release();
        }
    }

    if (!m_metadata.isEmpty())
        goto send_event;
#endif

#ifndef QT_NO_WMSDK
    IWMHeaderInfo *info = com_cast<IWMHeaderInfo>(source, IID_IWMHeaderInfo);

    if (info) {
        static const int  count = sizeof(qt_wmMetaDataKeys) / sizeof(QWMMetaDataKeyLookup);
        for (int i = 0; i < count; ++i) {
            QVariant var = getValue(info, qt_wmMetaDataKeys[i].token);
            if (var.isValid()) {
                QString key = qt_wmMetaDataKeys[i].key;

                if (key == QMediaMetaData::Duration) {
                    // duration is provided in 100-nanosecond units, convert to milliseconds
                    var = (var.toLongLong() + 10000) / 10000;
                } else if (key == QMediaMetaData::Resolution) {
                    QSize res;
                    res.setHeight(var.toUInt());
                    res.setWidth(getValue(info, L"WM/VideoWidth").toUInt());
                    var = res;
                } else if (key == QMediaMetaData::VideoFrameRate) {
                    var = var.toReal() / 1000.f;
                } else if (key == QMediaMetaData::PixelAspectRatio) {
                    QSize aspectRatio;
                    aspectRatio.setWidth(var.toUInt());
                    aspectRatio.setHeight(getValue(info, L"AspectRatioY").toUInt());
                    var = aspectRatio;
                } else if (key == QMediaMetaData::UserRating) {
                    var = (var.toUInt() - 1) / qreal(98) * 100;
                }

                m_metadata.insert(key, var);
            }
        }

        info->Release();
    }

    if (!m_metadata.isEmpty())
        goto send_event;
#endif
    {
        IAMMediaContent *content = 0;

        if ((!graph || graph->QueryInterface(
                 IID_IAMMediaContent, reinterpret_cast<void **>(&content)) != S_OK)
                && (!source || source->QueryInterface(
                        IID_IAMMediaContent, reinterpret_cast<void **>(&content)) != S_OK)) {
            content = 0;
        }

        if (content) {
            BSTR string = 0;

            if (content->get_AuthorName(&string) == S_OK)
                m_metadata.insert(QMediaMetaData::Author, convertBSTR(&string));

            if (content->get_Title(&string) == S_OK)
                m_metadata.insert(QMediaMetaData::Title, convertBSTR(&string));

            if (content->get_Description(&string) == S_OK)
                m_metadata.insert(QMediaMetaData::Description, convertBSTR(&string));

            if (content->get_Rating(&string) == S_OK)
                m_metadata.insert(QMediaMetaData::UserRating, convertBSTR(&string));

            if (content->get_Copyright(&string) == S_OK)
                m_metadata.insert(QMediaMetaData::Copyright, convertBSTR(&string));

            content->Release();
        }
    }

send_event:
    // DirectShowMediaPlayerService holds a lock at this point so defer emitting signals to a later
    // time.
    QCoreApplication::postEvent(this, new QEvent(QEvent::Type(MetaDataChanged)));
}

void DirectShowMetaDataControl::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type(MetaDataChanged)) {
        event->accept();

        bool oldAvailable = m_available;
        m_available = !m_metadata.isEmpty();
        if (m_available != oldAvailable)
            emit metaDataAvailableChanged(m_available);

        emit metaDataChanged();
    } else {
        QMetaDataReaderControl::customEvent(event);
    }
}