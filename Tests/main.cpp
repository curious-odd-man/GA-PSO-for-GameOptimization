#include "Common.hpp"


class MyEvaluator : public UtilityEvaluator
{
public:
    MyEvaluator(size_t p) : UtilityEvaluator(p) {};
    virtual ~MyEvaluator() {};
};


void do_test(bool success, string name, bool throw_on_error = false, bool spam_ok = false)
{
    if (success)
    {
        if (spam_ok)
        {
            cout << "OK! " << name.c_str() << endl;
            fflush(stdout);
        }
    }
    else
    {
        cout << "Error! " << name.c_str() << endl;
        fflush(stdout);
        if (throw_on_error)
            throw (-1);
    }
}

void testFigure()
{
    Figure f(3, 7);
    vector<Figure> v(f.getAllStates());

    cout << "var 2: initial " << f;

    try
    {
        do_test(v.size() == 3, "number of different figure states", true);

        do_test(v[0].getData()[0] == f.getData()[0], "Figure data test", false, false);
        do_test(v[0].getData()[1] == f.getData()[1], "Figure data test", false, false);
        do_test(v[0].getData()[2] == f.getData()[2], "Figure data test", false, false);

        do_test(v[1].getData()[0] == f.getData()[1], "Figure data test", false, false);
        do_test(v[1].getData()[1] == f.getData()[2], "Figure data test", false, false);
        do_test(v[1].getData()[2] == f.getData()[0], "Figure data test", false, false);

        do_test(v[2].getData()[0] == f.getData()[2], "Figure data test", false, false);
        do_test(v[2].getData()[1] == f.getData()[0], "Figure data test", false, false);
        do_test(v[2].getData()[2] == f.getData()[1], "Figure data test", false, false);

    }
    catch (const int& e)
    {
        cout << "pichal " << e << endl;
    }

    Figure ff(3, 7);
    const unsigned char * ptr = ff.begin();

    while (ptr != ff.end())
    {
        for (size_t i = 0; i < ff.getSize(); ++i)
        {
            // do something ( get figure bytes )
        }
        ptr++;
    }

    ptr = ff.begin();

    try
    {
        do_test(*ptr == ff.getData()[0], "Figure data test", false, false);
        do_test(*(ptr + 1) == ff.getData()[1], "Figure data test", false, false);
        do_test(*(ptr + 2) == ff.getData()[2], "Figure data test", false, false);

        do_test(*(ptr + 1) == ff.getData()[1], "Figure data test", false, false);
        do_test(*(ptr + 2) == ff.getData()[2], "Figure data test", false, false);
        do_test(*(ptr + 3) == ff.getData()[0], "Figure data test", false, false);

        do_test(*(ptr + 2) == ff.getData()[2], "Figure data test", false, false);
        do_test(*(ptr + 3) == ff.getData()[0], "Figure data test", false, false);
        do_test(*(ptr + 4) == ff.getData()[1], "Figure data test", false, false);

    }
    catch (const int& e)
    {
        cout << "pichal " << e << endl;
    }
}

class FieldTest : public Field
{
public:
    FieldTest(size_t width, size_t height, UtilityEvaluator* evaluator)
        : Field(width, height, evaluator)
    {

    }

    void printMask(const unsigned char* mask, size_t width, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
        {
            if (i % width == 0)
                cout << endl;
            cout << +mask[i] << ' ';
        }
        cout << endl;
    }

    void testNextStates()
    {
        // test next states
        Figure fig(3, 7);
        vector<Field> t;
        getNextStates(fig, t);
        do_test(t.size() == 21, "All possible figure variations");
        memset(aField, 1, aFieldSize);
        do_test(getNextStates(fig, t).size() == 0, "Can't add figure");
        memset(aField, 0, aWidth << 1);     // clear 2 upper lines
        do_test(getNextStates(fig, t).size() == 0, "Can't add figure 2 lines clear");
        // incrementally increase space for figures
        for (size_t i = 0; i < aWidth; ++i)
        {
            *(aField + (aWidth << 1) + i) = 0;
            do_test(getNextStates(fig, t).size() == 3 * (i + 1), "incremental test");
        }

        // clear some more cells on 4th line
        *(aField + (aWidth << 1) + aWidth) = 0;
        *(aField + (aWidth << 1) + aWidth + 3) = 0;
        *(aField + (aWidth << 1) + aWidth + 5) = 0;
        // there can't be more than aWidth * 3 new field states
        do_test(getNextStates(fig, t).size() == 3 * aWidth, "incremental test");
    }

    void testRemoveMaskFill()
    {
        bool changed = false;
        unsigned char * mask = new unsigned char[aFieldSize];
        for (size_t i = 0; i < aFieldSize; ++i)
            aField[i] = (unsigned char)i;

        changed = markRemovable(mask);
        do_test(changed == false, "remove mask: no changes");

        unsigned char test_init[] = {
            0, 0, 0, 0, 0, 0, 0,
            0, 7, 7, 0, 7, 7, 0,
            0, 0, 7, 7, 7, 0, 0,
            0, 0, 0, 7, 0, 0, 0,
            6, 6, 6, 6, 6, 0, 2,
            0, 0, 6, 0, 0, 0, 2,
            0, 0, 0, 0, 0, 0, 2,
            0, 0, 0, 0, 0, 0, 2,
            3, 3, 3, 3, 3, 0, 2,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 5, 4, 0, 0, 0,
            0, 0, 4, 5, 0, 0, 1,
            0, 4, 0, 0, 5, 0, 1,
            1, 1, 1, 0, 0, 0, 1
        };

        unsigned char test_result[] = {
            0, 0, 0, 0, 0, 0, 0,
            0, 7, 0, 0, 0, 7, 0,
            0, 0, 7, 7, 7, 0, 0,
            0, 0, 0, 7, 0, 0, 0,
            6, 6, 6, 6, 6, 0, 2,
            0, 0, 0, 0, 0, 0, 2,
            0, 0, 0, 0, 0, 0, 2,
            0, 0, 0, 0, 0, 0, 2,
            3, 3, 3, 3, 3, 0, 2,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 5, 4, 0, 0, 0,
            0, 0, 4, 5, 0, 0, 1,
            0, 4, 0, 0, 5, 0, 1,
            1, 1, 1, 0, 0, 0, 1,
        };

        memmove(aField, test_init, aFieldSize);
        changed = markRemovable(mask);
        do_test(changed == true, "remove mask: test case 1 - changed");
        do_test(memcmp(mask, test_result, aFieldSize) == 0 , "remove mask: test case 1 - result correctness");

        unsigned char test_init2[] = {
            2,2,2,1,2,2,2,
            2,4,1,0,1,4,2,
            2,0,4,1,4,0,2,
            0,0,0,4,0,0,0,
            0,0,4,0,4,0,0,
            0,4,0,0,0,4,0,
            4,4,4,4,4,4,4,
            1,0,1,1,0,0,1,
            5,5,5,5,5,0,1,
            3,0,5,0,0,2,0,
            0,3,5,0,2,0,0,
            1,0,1,0,1,0,1,
            1,1,0,0,0,1,1,
            0,1,1,0,1,1,0,
        };

        unsigned char test_result2[] = {
            2, 2, 2, 0, 2, 2, 2,
            2, 4, 0, 0, 0, 4, 2,
            2, 0, 4, 0, 4, 0, 2,
            0, 0, 0, 4, 0, 0, 0,
            0, 0, 4, 0, 4, 0, 0,
            0, 4, 0, 0, 0, 4, 0,
            4, 4, 4, 4, 4, 4, 4,
            0, 0, 0, 0, 0, 0, 0,
            5, 5, 5, 5, 5, 0, 0,
            0, 0, 5, 0, 0, 0, 0,
            0, 0, 5, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 1,
            0, 1, 0, 0, 0, 1, 0,
            0, 0, 1, 0, 1, 0, 0,

        };

        memmove(aField, test_init2, aFieldSize);
        changed = markRemovable(mask);
        do_test(changed == true, "remove mask: test case 2 - changed");
        do_test(memcmp(mask, test_result2, aFieldSize) == 0, "remove mask: test case 2 - result correctnes");


        unsigned char test3[] = {
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            0, 1, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 1, 0, 1,
        };

        memmove(aField, test3, aFieldSize);
        changed = markRemovable(mask);
        do_test(changed == true, "remove mask: test case 3 - changed");
        do_test(memcmp(mask, test3, aFieldSize) == 0, "remove mask: test case 3 - result correctnes");

        unsigned char test4[] = {
            1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 0,
            1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0,
            1, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 0, 1, 1, 1,
            0, 0, 0, 1, 1, 1, 0,
            1, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 1, 1, 1,
        };

        memmove(aField, test4, aFieldSize);
        changed = markRemovable(mask);
        do_test(changed == true, "remove mask: test case 4 - changed");
        do_test(memcmp(mask, test4, aFieldSize) == 0, "remove mask: test case 4 - result correctnes");

        unsigned char test5[] = {
            1, 0, 0, 0, 1, 0, 0,
            1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 0, 1,
            1, 0, 1, 0, 0, 1, 0,
            0, 1, 0, 1, 0, 0, 1,
            0, 0, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 0,
            1, 0, 0, 1, 0, 1, 0,
            0, 1, 0, 0, 1, 0, 1,
            0, 0, 1, 0, 1, 0, 0,
            0, 0, 0, 1, 0, 1, 0,
            1, 0, 0, 0, 1, 0, 1,
            0, 1, 0, 0, 0, 1, 0,
            0, 0, 1, 0, 0, 0, 1,
        };

        memmove(aField, test5, aFieldSize);
        changed = markRemovable(mask);
        do_test(changed == true, "remove mask: test case 5 - changed");
        do_test(memcmp(mask, test5, aFieldSize) == 0, "remove mask: test case 5 - result correctnes");

        unsigned char test6[] = {
            0, 0, 1, 0, 0, 0, 1,
            0, 1, 0, 0, 0, 1, 0,
            1, 0, 1, 0, 1, 0, 0,
            0, 1, 0, 0, 0, 1, 0,
            1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 1, 0, 0, 1,
            0, 1, 1, 0, 0, 1, 0,
            1, 1, 0, 0, 1, 0, 1,
            1, 0, 0, 1, 0, 1, 0,
            0, 0, 1, 0, 1, 0, 0,
            0, 1, 0, 1, 0, 0, 1,
            1, 0, 1, 0, 0, 1, 1,
            0, 1, 0, 0, 1, 1, 0,
            1, 0, 0, 1, 1, 0, 0,
        };

        memmove(aField, test6, aFieldSize);
        changed = markRemovable(mask);
        do_test(changed == true, "remove mask: test case 6 - changed");
        do_test(memcmp(mask, test6, aFieldSize) == 0, "remove mask: test case 6 - result correctnes");

        delete[] mask;
    }

    void testRemoveAndCompact()
    {
        unsigned char * tmp = new unsigned char[aFieldSize];

        for (size_t i = 0; i < aFieldSize; ++i)
            aField[i] = (unsigned char)i;

        compact();
        memmove(tmp, aField, aFieldSize);
        removeColors();
        do_test(memcmp(tmp, aField, aFieldSize) == 0, "Remove: no changes");

        const unsigned char tests[] = {
        // test 1
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0,
                3, 2, 3, 2, 3, 2, 3,
                1, 1, 1, 1, 1, 1, 1,
            // test 2
            4, 5, 4, 5, 4, 5, 4,
            2, 3, 2, 3, 2, 3, 2,
            3, 2, 3, 2, 3, 2, 3,
            1, 1, 1, 1, 1, 1, 1,
            5, 4, 5, 4, 5, 4, 5,
            4, 5, 4, 5, 4, 5, 4,
            2, 3, 2, 3, 2, 3, 2,
            3, 2, 3, 2, 3, 2, 3,
            1, 1, 1, 1, 1, 1, 1,
            5, 4, 5, 4, 5, 4, 5,
            4, 5, 4, 5, 4, 5, 4,
            2, 3, 2, 3, 2, 3, 2,
            3, 2, 3, 2, 3, 2, 3,
            1, 1, 1, 1, 1, 1, 1,
            // test 3
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 2, 0, 0, 0,
            0, 0, 2, 6, 2, 0, 0,
            0, 0, 1, 5, 1, 0, 0,
            0, 2, 4, 4, 4, 2, 0,
            2, 1, 2, 1, 2, 1, 2,
            1, 2, 1, 3, 1, 2, 1,

        };

        const unsigned char results[] =  {
        // result 1
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            3, 2, 3, 2, 3, 2, 3,
        // result 2
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            4, 5, 4, 5, 4, 5, 4,
            2, 3, 2, 3, 2, 3, 2,
            3, 2, 3, 2, 3, 2, 3,
            5, 4, 5, 4, 5, 4, 5,
            4, 5, 4, 5, 4, 5, 4,
            2, 3, 2, 3, 2, 3, 2,
            3, 2, 3, 2, 3, 2, 3,
            5, 4, 5, 4, 5, 4, 5,
            4, 5, 4, 5, 4, 5, 4,
            2, 3, 2, 3, 2, 3, 2,
            3, 2, 3, 2, 3, 2, 3,
            // result 3
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 6, 0, 0, 0,
            0, 0, 0, 5, 0, 0, 0,
            0, 2, 2, 3, 2, 2, 0,

        };

        const size_t removed_count[] = {
            7, 21, 19
        };

        for (size_t i = 0; i < sizeof(tests) / aFieldSize; ++i)
        {
            memmove(aField, tests + (i * aFieldSize), aFieldSize);
            //cout << *this;
            removeColors();
            //cout << *this;
            stringstream ss;
            ss << "removeColors test #" << i;
            do_test(memcmp(results + (i * aFieldSize), aField, aFieldSize) == 0, ss.str());
            ss << "count";
            do_test(removed_count[i] == aRemoved, ss.str());
        }


        delete[] tmp;
    }

    void testRemoval()
    {
        testRemoveMaskFill();
        testRemoveAndCompact();
    }

    void testEvaluation()
    {
        ofstream out;
        out.open("Test.log");

        const unsigned char tests[] = {
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 2, 0, 0, 0,
            0, 0, 2, 6, 2, 0, 0,
            0, 0, 1, 5, 1, 0, 0,
            0, 2, 4, 7, 4, 2, 0,
            2, 1, 2, 1, 2, 1, 2,
            1, 2, 1, 3, 1, 2, 1,
        };

        memcpy(aField, tests, aFieldSize);
        Figure fig(3, 7);
        vector<Field> t;
        getNextStates(fig, t);

        for (auto f : t)
            out << f;
        out.close();
    }
};

void testField()
{
    MyEvaluator e(6);
    FieldTest f(7, 14, &e);
    f.testNextStates();
    f.testRemoval();
    f.testEvaluation();
}


int main(int argc, char** argv)
{
    srand((unsigned int)time(NULL));      // note this need to be called once for a program

    testFigure();
    cout << "figure ended" << endl;
    testField();
    cout << "all ended" << endl << " Test game " << endl;
    MyEvaluator e(6);
    //DemonstrationGame dg(&e, 7, 14, 3, 7);
    //dg.play();
    cout << "game ended " << endl;
    cin.get();
}
