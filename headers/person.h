#ifndef PERSON_H
#define PERSON_H

class Person {
  public:
    Person() {}
    virtual void update()=0;
    void draw();

  protected:
    //void update_pos();

  private:
    int x, y, vx, vy;
};

#endif // PERSON_H
