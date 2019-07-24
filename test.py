import subprocess
import argparse


def read_input(file):
    with open(file) as f:
        text = f.read()
        return [f'{case}\n'.encode() for case in text.split('\n\n')]


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('testcase-file', type=str)
    parser.add_argument('programs', type=str, nargs='+')
    parser.add_argument('--n-round', type=int, default=10)
    args = parser.parse_args()
    n_round = args.n_round
    test_cases = read_input(getattr(args, 'testcase-file'))
    print(f'{n_round} rounds')
    print(f'{len(test_cases)} test cases\n')
    for program in args.programs:
        print(program)
        times = []
        for i in range(n_round):
            time = 0
            for test_case in test_cases:
                p = subprocess.run(program, input=test_case, capture_output=True)
                time += float(p.stdout.decode().split('\n')[0].split(':')[-1].strip())
            times.append(time)
        print(f'{min(times):.2f}/{sum(times)/n_round:.2f}/{max(times):.2f} min/avg/max ms per round\n')
