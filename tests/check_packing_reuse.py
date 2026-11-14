import os, subprocess, tempfile
from pathlib import Path
root = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix='packing-check-') as tmp:
    bins = []
    for name, source in [('original', 'src/original/Matrix_mul.c'), ('reuse', 'src/packing_reuse/Matrix_mul_pack_reuse.c')]:
        binary = str(Path(tmp)/name)
        subprocess.run(['gcc','-std=gnu99','-O3','-mavx','-fopenmp','-ffp-contract=off','-Wall','-Wextra',str(root/source),'-o',binary],check=True)
        bins.append(binary)
    count = 0
    for threads in [1,2,4]:
        env = dict(os.environ, OMP_NUM_THREADS=str(threads))
        for n in [1,3,4,15,16,17,63,64,65,127,128,129,191,192,193,255,256,257]:
            for seed in ['0.123','0.314159','0.7']:
                runs = [subprocess.run([b,str(n),seed],env=env,text=True,capture_output=True,check=True,timeout=60) for b in bins]
                assert runs[0].stdout == runs[1].stdout, (threads,n,seed,[r.stdout for r in runs])
                assert 'elapsed_sec=' in runs[1].stderr
                count += 1
    for n, seed in [('0','0.1'),('-1','0.1'),('abc','0.1'),('12x','0.1'),('2147483647','0.1'),('4','nan'),('4','junk')]:
        run = subprocess.run([bins[1],n,seed],capture_output=True)
        assert run.returncode != 0 and not run.stdout
    print(f'PASS: {count} exact Trace comparisons; 7 invalid-input checks. Local correctness only; no server performance claim.')
