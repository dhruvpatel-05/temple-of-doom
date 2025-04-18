//
//  Coord.h
//  Proj3
//
//  Created by Dhruv Patel on 5/18/24.
//

#ifndef Coord_h
#define Coord_h
class Coord
{
  public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
    void setr(int r) {m_row = r; }
    void setc(int c) {m_col = c; }
  private:
    int m_row;
    int m_col;
};
//used for storing info about the position of actors on the grid
#endif /* Coord_h */
