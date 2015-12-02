// Copyright (c) 2015, Daniel Pfeifer <daniel@pfeifer-mail.de>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <_test.hpp>
#include <byom/ext/qobject.hpp>
#include <QSharedPointer>

class QPerson : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(int age READ age WRITE setAge)

public:
  QPerson(QObject* parent = nullptr)
    : QObject(parent)
  {
  }

  QString name() const
  {
    return m_name;
  }

  int age() const
  {
    return m_age;
  }

  void setName(QString name)
  {
    m_name = name;
  }

  void setAge(int age)
  {
    m_age = age;
  }

private:
  QString m_name;
  int m_age;
};

#include "qobject.moc"

int qobject(int argc, char* argv[])
{
  auto model = QSharedPointer<QPerson>::create();
  model->setName("John Doe");
  model->setAge(42);

  return test_map(model.data());
}
