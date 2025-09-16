#include "grade.h"

Grade::Grade(const int &total)
{
    reset(total);
}

void Grade::insert(const int &type)
{
    switch (type)
    {
    case 3:
        // perfect
        score1 += root1, key *= root2;
        break;
    case 2:
        // good
        score1 += root1 * 0.75f, key *= root2;
        break;
    case 1:
    case 0:
        score2 += key, key = 1.0f;
        break;
    default:
        break;
    }
    return;
}

int Grade::getGrade(void)
{
    score2 += key, key = 1.0f;
    return int(1000000 * (score1) + 0 * (score2 - 1.0f));
}

void Grade::reset(const int &total)
{
    root1 = 1 / static_cast<float>(total), root2 = std::exp2(root1), score1 = score2 = 0.0f, key = 1.0f;
    std::cerr << "[Info]:Root is [" << root1 << ", " << root2 << "]." << std::endl;
    return;
}

