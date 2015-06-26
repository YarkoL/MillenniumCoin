#ifndef AESQT_H
#define AESQT_H

#include <QString>

class Aesqt
{
public:
    Aesqt();
    inline int getAlignedSize(int currSize, int alignment);
    QString encodeText(const QString &rawText, const QString &key);
    QString decodeText(const QString &hexEncodedText, const QString &key);
};

#endif // AESQT_H
