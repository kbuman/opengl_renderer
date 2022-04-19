//
// Created by kevin on 13/12/2021.
//

#ifndef OPENGL_CPP_BUFFER_INDEX_BUFFER_H_
#define OPENGL_CPP_BUFFER_INDEX_BUFFER_H_

class IBuffer {
 public:
  IBuffer(const unsigned int *, unsigned int, bool = true);
  ~IBuffer();

  void Bind() const;
  void Unbind() const;
  [[nodiscard]] unsigned int Count() const { return m_count; }
 private:
  unsigned int m_programID;
  const unsigned int *m_data;
  unsigned int m_count;
  bool m_static;
};
#endif //OPENGL_CPP_BUFFER_INDEX_BUFFER_H_
