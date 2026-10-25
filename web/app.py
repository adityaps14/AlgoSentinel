from flask import Flask, render_template, request, jsonify
from pathlib import Path
import subprocess
import csv
import io
import threading

app = Flask(__name__)

# All website files remain inside the web folder.
BASE_DIR = Path(__file__).resolve().parent
ENGINE_SOURCE = BASE_DIR / "benchmark_engine.cpp"
ENGINE_EXE = BASE_DIR / "benchmark_engine.exe"
REPORT_FILE = BASE_DIR / "benchmark_results.csv"

# Protect compilation and report writing from simultaneous requests.
engine_lock = threading.Lock()

ALLOWED_ALGORITHMS = {"all", "bubble", "insertion", "merge"}
ALLOWED_SIZES = {1000, 5000, 10000}


def compile_engine():
    """Compile the C++ benchmark engine when needed."""
    if not ENGINE_SOURCE.exists():
        raise RuntimeError(
            "benchmark_engine.cpp nahi mili. Check karo ki file web folder mein hai."
        )

    needs_compile = (
        not ENGINE_EXE.exists()
        or ENGINE_EXE.stat().st_mtime < ENGINE_SOURCE.stat().st_mtime
    )

    if needs_compile:
        result = subprocess.run(
            [
                "g++",
                "-std=c++11",
                "-O2",
                str(ENGINE_SOURCE),
                "-o",
                str(ENGINE_EXE),
            ],
            capture_output=True,
            text=True,
            timeout=60,
            cwd=str(BASE_DIR),
            check=False,
        )

        if result.returncode != 0:
            details = result.stderr.strip() or "Unknown compiler error"
            raise RuntimeError("C++ compilation failed: " + details)


def save_results(size, results):
    """Save website benchmark results separately from existing project reports."""
    file_exists = REPORT_FILE.exists()

    with REPORT_FILE.open("a", newline="", encoding="utf-8") as csv_file:
        writer = csv.writer(csv_file)

        if not file_exists or REPORT_FILE.stat().st_size == 0:
            writer.writerow([
                "input_size",
                "algorithm",
                "time_ms",
                "comparisons",
                "swaps",
            ])

        for result in results:
            writer.writerow([
                size,
                result["algorithm"],
                result["time_ms"],
                result["comparisons"],
                result["swaps"],
            ])


@app.route("/")
def home():
    return render_template("index.html")


@app.route("/api/benchmark", methods=["POST"])
def benchmark():
    data = request.get_json(silent=True) or {}

    algorithm = data.get("algorithm")
    size = data.get("size")

    if algorithm not in ALLOWED_ALGORITHMS:
        return jsonify({
            "error": "Invalid algorithm selected."
        }), 400

    # Reject booleans and strings instead of silently accepting them as sizes.
    if isinstance(size, bool) or not isinstance(size, int):
        return jsonify({
            "error": "Input size must be a valid integer."
        }), 400

    if size not in ALLOWED_SIZES:
        return jsonify({
            "error": "Allowed input sizes are 1000, 5000 and 10000."
        }), 400

    if size == 10000 and algorithm in {"all", "bubble", "insertion"}:
        return jsonify({
            "error": (
                "Bubble/Insertion Sort at 10000 elements can take longer. "
                "Choose Merge Sort for 10000, or use 1000/5000 for comparisons."
            )
        }), 400

    try:
        with engine_lock:
            compile_engine()

            completed = subprocess.run(
                [
                    str(ENGINE_EXE),
                    algorithm,
                    str(size),
                ],
                capture_output=True,
                text=True,
                timeout=90,
                cwd=str(BASE_DIR),
                check=False,
            )

            if completed.returncode != 0:
                details = completed.stderr.strip() or "Unknown C++ engine error"
                return jsonify({
                    "error": "C++ benchmark failed: " + details
                }), 500

            reader = csv.DictReader(io.StringIO(completed.stdout))
            results = []

            for row in reader:
                results.append({
                    "algorithm": row["algorithm"],
                    "time_ms": float(row["time_ms"]),
                    "comparisons": int(row["comparisons"]),
                    "swaps": int(row["swaps"]),
                })

            if not results:
                return jsonify({
                    "error": "C++ engine returned no benchmark results."
                }), 500

            save_results(size, results)

        return jsonify({
            "success": True,
            "input_size": size,
            "results": results,
        })

    except subprocess.TimeoutExpired:
        return jsonify({
            "error": "Benchmark took too long. Try a smaller input size."
        }), 408

    except FileNotFoundError as error:
        return jsonify({
            "error": (
                "Required program nahi mila. Check karo ki g++ PATH mein hai. "
                + str(error)
            )
        }), 500

    except (OSError, RuntimeError, ValueError, KeyError) as error:
        app.logger.exception("Benchmark request failed")
        return jsonify({
            "error": str(error)
        }), 500


if __name__ == "__main__":
    print("=" * 48)
    print("       ALGOSENTINEL PERFORMANCE LAB")
    print("=" * 48)
    print("Starting Flask server...")
    print("Open http://127.0.0.1:5000 in your browser.")
    print("Press Ctrl+C in this terminal to stop the server.")

    app.run(
        host="127.0.0.1",
        port=5001,
        debug=False,
    )