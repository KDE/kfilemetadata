#ifndef TAGLIBWRITER_H
#define TAGLIBWRITER_H

#include "writerplugin.h"

namespace KFileMetaData
{

class TagLibWriter : public WriterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.WriterPlugin")
    Q_INTERFACES(KFileMetaData::WriterPlugin)

public:
    TagLibWriter(QObject* parent = 0);

    void write(const WriteData& data) Q_DECL_OVERRIDE;
    QStringList writeMimetypes() const Q_DECL_OVERRIDE;
};

}

#endif // TAGLIBWRITER_H

