import subprocess
import argparse
import re

TIME_RE = re.compile(r'time:\s*(\d+(\.\d+)?)')


def read_input(file):
    with open(file) as f:
        text = f.read()
        return [f'{case}\n'.encode() for case in text.split('\n\n')]


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('testcase-file', type=str)
    parser.add_argument('programs', type=str, nargs='+')
    parser.add_argument('--n-round', type=int, default=10)
    parser.add_argument('--group-by', type=str, choices=['round', 'testcase'], default='round')
    args = parser.parse_args()
    n_round = args.n_round
    test_cases = read_input(getattr(args, 'testcase-file'))
    n_test_cases = len(test_cases)
    print(f'{n_round} rounds')
    print(f'{n_test_cases} test cases\n')
    for program in args.programs:
        print(program)
        times = []
        for i in range(n_round):
            for test_case in test_cases:
                p = subprocess.run(program, input=test_case, capture_output=True)
                lines = p.stdout.decode().split('\n')
                for line in lines:
                    reg_res = TIME_RE.match(line)
                    if reg_res:
                        time = float(reg_res.group(1))
                        times.append(time)
                        break
        if args.group_by == 'round':
            grouped_times = [sum(times[i * n_test_cases:(i + 1) * n_test_cases]) for i in range(n_round)]
            print(f'min/avg/max = {min(grouped_times):.2f}/{sum(grouped_times)/n_round:.2f}/{max(grouped_times):.2f} ms per round\n')
        else:
            grouped_times = [sum(times[i::n_test_cases]) for i in range(n_test_cases)]
            for i, time in enumerate(grouped_times):
                print(f'  test case {i+1}: {time / n_round:.4f} ms')
