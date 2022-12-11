
// If pr, qs intersect
if (CGAL::assign(segm_int, obj) || CGAL::assign(point_int, obj))
{
    polygonChainCopy = polygonChain;
    continue;
}

bool_intersect = false;
for (auto i1 = result.begin(); i1 != result.end(); ++i1)
{
    for (auto i = polygonChain.edges_begin(); i != polygonChain.edges_end(); i++)
    {
        if (i->source() == *i1 || i->target() == *i1)
        {
            Segment_2 test_int(i->source(), i->target());
            CGAL::Object obj1 = intersection(pr, test_int);
            CGAL::Object obj2 = intersection(qs, test_int);

            if (CGAL::assign(segm_int, obj1) || CGAL::assign(segm_int, obj2))
            {
                bool_intersect = true;
                cout << test_int << " intersectted with " << pr << " or with " << qs << endl;
                break;
            }

            if (const Point_2 *point = CGAL::object_cast<Point_2>(&obj1))
            {
                if (*point != i->source() && *point != i->target())
                {
                    bool_intersect = true;
                    cout << test_int << " intersectted with " << pr << " or with " << qs << endl;
                    break;
                }
            }

            if (const Point_2 *point = CGAL::object_cast<Point_2>(&obj2))
            {
                if (*point != i->source() && *point != i->target())
                {
                    bool_intersect = true;
                    cout << test_int << " intersectted with " << pr << " or with " << qs << endl;
                    break;
                }
            }
        }
    }
    if (bool_intersect)
        break;
}