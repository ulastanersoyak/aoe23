#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct parsed_args
{
  std::vector<std::int64_t> first_col;
  std::vector<std::int64_t> second_col;
};

[[nodiscard]] parsed_args
parse_file (std::string_view contents) noexcept
{
  parsed_args args{};

  for (const auto &line : std::views::split (contents, '\n'))
    {
      if (line.empty ())
        {
          break;
        }

      const std::string_view line_view{ line.begin (), line.end () };

      const std::string_view first_number_view{
        line_view.begin (), line_view.find_first_of (' ')
      };

      const std::string_view second_number_view{
        line_view.begin () + line_view.find_last_of (' ') + 1, line_view.end ()
      };

      args.first_col.push_back (std::stoul (first_number_view.data ()));
      args.second_col.push_back (std::stoul (second_number_view.data ()));
    }
  return args;
}

int
main (int argc, char **argv)
{
  if (argc < 2)
    {
      std::println ("usage: {} <input_path>", argv[0]);
      return EXIT_FAILURE;
    }

  std::ifstream file (argv[1]);
  std::stringstream buffer;
  buffer << file.rdbuf ();
  const std::string content = buffer.str ();

  auto args = parse_file (content);

  std::ranges::sort (args.first_col);
  std::ranges::sort (args.second_col);

  std::unordered_map<std::int64_t, std::int64_t> lookup{};

  std::size_t score{ 0 };

  for (std::size_t i = 0; i < args.first_col.size (); ++i)
    {
      if (lookup.contains (args.first_col.at (i)))
        {
          score += lookup.at (args.first_col.at (i)) * args.first_col.at (i);
        }
      else
        {
          const auto occur
              = std::ranges::count (args.second_col, args.first_col.at (i));
          lookup.insert ({ args.first_col.at (i), occur });
          score += args.first_col.at (i) * occur;
        }
    }

  std::println ("{}", score);
  return EXIT_SUCCESS;
}
