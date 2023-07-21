#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Envelope_diagram_1.h>
#include <CGAL/envelope_2.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel> Traits;
typedef Traits::Point_2 Point_2;
typedef Traits::X_monotone_curve_2 Segment_2;
typedef CGAL::Envelope_diagram_1<Traits> Diagram_1;

int main(int argc, char* argv[]) {

    using namespace nlohmann;

    if (argc != 2) {
        std::cerr <<"Pass line segments as JSON arrays. For example, if you had 2 segments, where the 0th segment endpoints are (0,0) and (1,0), and 1st are (1,1) and (2,1), then use this program like '$ envelope \"[ [[0,0], [1,0]], [[1, 1], [2, 1]] ]\" ' \t"<< __FILE__ << ":" << __LINE__ << std::endl;
        return 1;
    }
    std::string jsonstring {argv[1]};
    json j = json::parse(jsonstring);


    std::vector<Segment_2> segments;
    // Convert the json string to a vector
    if (!j.is_array()) throw std::runtime_error {"top level JSON object not an array"};

    for (const json& segment: j) {
        if (!segment.is_array()) throw std::runtime_error {"2nd level JSON object not an array"};
        if (segment.size() != 2) throw std::runtime_error {"Segment has more than two end points"};

        const std::array<json,2> points = {
                segment[0],
                segment[1],
        };

        Segment_2 segment_2;

        for (int i = 0; i < 2; ++i) {
            if (!segment.is_array()) throw std::runtime_error {"3rd level JSON object not an array"};
            if (segment.size() != 2) throw std::runtime_error {"A point is not in 2D"};
            segment_2 = Segment_2 {
                Point_2 {
                        points[0][0].get<double>(),
                        points[0][1].get<double>(),
                },
                Point_2 {
                        points[1][0].get<double>(),
                        points[1][1].get<double>(),
                },
            };
        }

        segments.push_back(segment_2);
    }

    Diagram_1 min_diag;
    CGAL::lower_envelope_x_monotone_2(segments.begin(), segments.end(), min_diag);

    Diagram_1::Edge_const_handle     e = min_diag.leftmost();

    json out_edges = json::array();

    while (e != min_diag.rightmost())
    {
        if (! e->is_empty())
        {
            for (auto cit = e->curves_begin(); cit != e->curves_end(); ++cit) {
                json edge = {
                        // left endpoint
                        {
                                CGAL::to_double(cit->left().x()),
                                CGAL::to_double(cit->left().y())
                        },
                        {
                                CGAL::to_double(cit->right().x()),
                                CGAL::to_double(cit->right().y())
                        }
                };
                out_edges.emplace_back(edge);
            }
        }
//        else
//            std::cout << " [empty]";

        auto v = e->right(); // vertex
        e = v->right();
    }

    std::cout << out_edges;

    return 0;
}
