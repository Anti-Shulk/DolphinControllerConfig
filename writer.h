#ifndef WRITER_H
#define WRITER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

class Writer {
public:
    enum Mode {
        REPLACE_FULL_LINE, REPLACE_TEXT_ONLY
    };
private:
    QString filePath;
    QFile originalFile;
    Mode mode = REPLACE_TEXT_ONLY;
    QFile tempFile;



public:
    Writer(QWidget* parent, Mode mode, const QString& filePath) :
        filePath(filePath),
        originalFile(filePath),
        mode(mode),
        tempFile(filePath + ".tmp")
    {
        if (!originalFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                QMessageBox::warning(parent, parent->tr("Warning"), parent->tr("Unable to open file: ") + originalFile.errorString() +
                                     "The file path is: " + filePath +
                                     "Make sure your paths are configured incorrectly. "
                                     "For more information, please read README.md or see "
                                     "https://github.com/Anti-Shulk/DolphinControllerConfig");
                QApplication::quit();
                delete parent;
                return;
        }

        if (!tempFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                QMessageBox::warning(parent, parent->tr("Warning"), parent->tr("Unable to create temporary file: ") + tempFile.errorString() +
                                     "The file path of the original file is: " + filePath +
                                     "Make sure your paths are configured incorrectly. "
                                     "For more information, please read README.md or see "
                                     "https://github.com/Anti-Shulk/DolphinControllerConfig");
                QApplication::quit();
                delete parent;
                return;
        }
    }

    ~Writer()
    {
        originalFile.close();
        tempFile.remove();
    }

    void setMode(Mode mode)
    {
        this->mode = mode;
    }

    void replaceFileText(const QString& toBeReplaced, const QString& toReplaceWith)
    {
        QTextStream originalFileStream(&originalFile);
        QTextStream tempFileStream(&tempFile);

        // reset the stream to the beginning positions
        tempFileStream.seek(0);
        originalFileStream.seek(0);

        QString line;

        // go through the original stream and replace the text with the text that is being requested to be replaced
        while (!originalFileStream.atEnd()) {
            line = originalFileStream.readLine();
            if (line.contains(toBeReplaced)) { // If it contains the text we want to replace

                switch (mode) {
                case (REPLACE_FULL_LINE):
                    tempFileStream << toReplaceWith + "\n"; // Replace the entire line
                    break;
                default:
                case (REPLACE_TEXT_ONLY):
                    tempFileStream << line.replace(toBeReplaced, toReplaceWith) + "\n"; // Replace only the text
                    break;
                }

            } else { // If the line is normal just copy the line from the original to the temp stream
                tempFileStream << line + "\n";
            }

        }

        // reset the stream to the beginning positions
        tempFileStream.seek(0);
        originalFileStream.seek(0);

        // write all the data from the temp stream back to the original file
        while (!tempFileStream.atEnd()) {
            line = tempFileStream.readLine();
            originalFileStream << line + "\n";
        }
    }
    void replaceFileTextAndSkipLines(const QString& skipStart, const QString& skipEnd, const QString& toReplaceWith) {

        QTextStream originalFileStream(&originalFile);
        QTextStream tempFileStream(&tempFile);

        // reset the stream to the beginning positions
        tempFileStream.seek(0);
        originalFileStream.seek(0);

        QString line;

        while (!originalFileStream.atEnd()) { // copy file as normal
            line = originalFileStream.readLine();
            tempFileStream << line + "\n";                   // copy over the line
            if (line.contains(skipStart)) break; // if that line we just copied contains replacement start, break out of while loop
        }

        tempFileStream << toReplaceWith + "\n"; // copy over all the data to be replace. if this contains \n it will allow multiple lines

        while (!originalFileStream.atEnd()) { // continue going through the original file till the toBeReplacedEnd is found
            line = originalFileStream.readLine();
            if (line.contains(skipEnd)) { // once it is, we copy over the line containing it and break out of while loop
                tempFileStream << line + "\n";
                break;
            }


        }

        while (!originalFileStream.atEnd()) // finally the rest of the original file stream is copied into the temp stream
            tempFileStream << originalFileStream.readLine() + "\n";

        // reset both streams to get ready to write into original file
        tempFileStream.seek(0);
        originalFileStream.seek(0);


        while (!tempFileStream.atEnd()) // write the temp stream into original file
            originalFileStream << tempFileStream.readLine() + "\n";

    }

};
class FileReader {
public:
    static QString fileToString(QWidget* parent, const QString& filePath) // take a file and convert it to a stream with \n for each line also
    {
        QString contents;
        QFile file(filePath);

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                QMessageBox::warning(parent, parent->tr("Warning"), parent->tr("Unable to open file: ") + file.errorString() +
                                     "The file path is: " + filePath +
                                     "Make sure your paths are configured incorrectly. "
                                     "For more information, please read README.md or see "
                                     "https://github.com/Anti-Shulk/DolphinControllerConfig");
                return nullptr;
        }

        QTextStream stream(&file);
        while (!stream.atEnd()) {
            contents.append(stream.readLine() + "\n");
        }
        file.close();
        return contents;
    }

};

//class WriterBuilder {
//private:
//    QWidget* parent, Mode mode, QString filePath
//public:
//    WriterBuilder(QWidget* parent)
//};

#endif // WRITER_H
