"use strict";

/* ==========================================
   ALGOSENTINEL - FRONTEND JAVASCRIPT
   ========================================== */

const $ = (selector) => document.querySelector(selector);

let benchmarkChart = null;
let currentArray = [];
let isSorting = false;
let operationCounter = 0;

/* ==========================================
   NAVIGATION
   ========================================== */

document.querySelectorAll(".nav-link").forEach((link) => {
    link.addEventListener("click", (event) => {
        const href = link.getAttribute("href");

        if (href && href.startsWith("#")) {
            const target = document.querySelector(href);

            if (target) {
                event.preventDefault();
                target.scrollIntoView({
                    behavior: "smooth",
                    block: "start"
                });

                document.querySelectorAll(".nav-link").forEach((item) => {
                    item.classList.remove("active");
                });

                link.classList.add("active");
            }
        }
    });
});

/* ==========================================
   BENCHMARK FORM
   Actual results will come from the Flask API.
   ========================================== */

const benchmarkForm = $("#benchmarkForm");
const benchmarkButton = $("#runBenchmark");
const benchmarkStatus = $("#benchmarkStatus");
const benchmarkResults = $("#benchmarkResults");

if (benchmarkForm) {
    benchmarkForm.addEventListener("submit", async (event) => {
        event.preventDefault();
        await runBenchmark();
    });
}

async function runBenchmark() {
    if (!benchmarkForm || !benchmarkButton) return;

    const algorithmElement = $('[name="algorithm"]');
    const sizeElement = $('[name="size"]');

    if (!algorithmElement || !sizeElement) {
        showBenchmarkStatus(
            "Benchmark form mein algorithm ya input-size field nahi mili.",
            true
        );
        return;
    }

    const algorithm = algorithmElement.value;
    const size = Number(sizeElement.value);

    if (!Number.isInteger(size) || size < 1) {
        showBenchmarkStatus("Please valid input size select karo.", true);
        return;
    }

    benchmarkButton.disabled = true;
    benchmarkButton.textContent = "Running benchmark...";
    showBenchmarkStatus(
        "C++ engine se benchmark run ho raha hai. Please wait..."
    );

    if (benchmarkResults) {
        benchmarkResults.innerHTML = "";
    }

    try {
        const response = await fetch("/api/benchmark", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                algorithm: algorithm,
                size: size
            })
        });

        const data = await response.json().catch(() => ({}));

        if (!response.ok) {
            throw new Error(
                data.error || `Server error (${response.status})`
            );
        }

        if (!Array.isArray(data.results)) {
            throw new Error("Server se expected benchmark results nahi mile.");
        }

        renderBenchmarkResults(data.results);
        updateBenchmarkChart(data.results);

        showBenchmarkStatus(
            `Benchmark complete! Input size: ${formatNumber(size)} elements.`
        );

    } catch (error) {
        console.error("Benchmark error:", error);

        showBenchmarkStatus(
            error.message ||
            "Benchmark run nahi hua. Flask server aur C++ engine check karo.",
            true
        );

    } finally {
        benchmarkButton.disabled = false;
        benchmarkButton.textContent = "▶ Run Benchmark";
    }
}

function showBenchmarkStatus(message, isError = false) {
    if (!benchmarkStatus) return;

    benchmarkStatus.textContent = message;
    benchmarkStatus.style.color = isError ? "#fb7185" : "#91a0b9";
}

function renderBenchmarkResults(results) {
    if (!benchmarkResults) return;

    if (results.length === 0) {
        benchmarkResults.innerHTML =
            '<div class="empty-state">No benchmark results available.</div>';
        return;
    }

    const table = document.createElement("table");
    table.className = "result-table";

    const thead = document.createElement("thead");
    const headerRow = document.createElement("tr");

    ["Algorithm", "Time (ms)", "Comparisons", "Swaps"].forEach((label) => {
        const th = document.createElement("th");
        th.textContent = label;
        headerRow.appendChild(th);
    });

    thead.appendChild(headerRow);
    table.appendChild(thead);

    const tbody = document.createElement("tbody");

    results.forEach((result) => {
        const row = document.createElement("tr");

        const values = [
            result.algorithm ?? "Unknown",
            formatTime(result.time_ms),
            formatNumber(result.comparisons),
            formatNumber(result.swaps)
        ];

        values.forEach((value) => {
            const td = document.createElement("td");
            td.textContent = String(value);
            row.appendChild(td);
        });

        tbody.appendChild(row);
    });

    table.appendChild(tbody);
    benchmarkResults.replaceChildren(table);
}

function updateBenchmarkChart(results) {
    const canvas = $("#benchmarkChart");

    if (!canvas || typeof Chart === "undefined") {
        console.warn("Chart.js ya benchmarkChart canvas nahi mila.");
        return;
    }

    const emptyMessage = $("#chartEmpty");

    if (emptyMessage) {
        emptyMessage.style.display = results.length ? "none" : "block";
    }

    const labels = results.map((item) => item.algorithm);
    const times = results.map((item) => Number(item.time_ms) || 0);

    if (benchmarkChart) {
        benchmarkChart.destroy();
    }

    benchmarkChart = new Chart(canvas, {
        type: "bar",
        data: {
            labels: labels,
            datasets: [{
                label: "Execution Time (ms)",
                data: times,
                backgroundColor: [
                    "rgba(34, 211, 238, 0.78)",
                    "rgba(100, 133, 255, 0.78)",
                    "rgba(52, 211, 153, 0.78)"
                ],
                borderColor: [
                    "#22d3ee",
                    "#6485ff",
                    "#34d399"
                ],
                borderWidth: 1,
                borderRadius: 7,
                maxBarThickness: 65
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            animation: {
                duration: 500
            },
            plugins: {
                legend: {
                    labels: {
                        color: "#cbd5e1",
                        font: {
                            family: "Inter"
                        }
                    }
                },
                tooltip: {
                    callbacks: {
                        label: (context) =>
                            `${context.parsed.y.toFixed(3)} ms`
                    }
                }
            },
            scales: {
                x: {
                    ticks: {
                        color: "#91a0b9"
                    },
                    grid: {
                        color: "rgba(148, 163, 184, 0.08)"
                    }
                },
                y: {
                    beginAtZero: true,
                    ticks: {
                        color: "#91a0b9"
                    },
                    grid: {
                        color: "rgba(148, 163, 184, 0.10)"
                    },
                    title: {
                        display: true,
                        text: "Milliseconds",
                        color: "#91a0b9"
                    }
                }
            }
        }
    });
}

/* ==========================================
   SORTING VISUALIZER
   Educational animation in the browser.
   The benchmark above will use the C++ backend.
   ========================================== */

const arraySizeInput = $("#arraySize");
const arraySizeLabel = $("#arraySizeLabel");
const arrayBars = $("#arrayBars");
const generateArrayButton = $("#generateArray");
const startVisualizationButton = $("#startVisualization");
const sortAlgorithmSelect = $("#sortAlgorithm");
const visualizerStatus = $("#visualizerStatus");
const operationCountDisplay = $("#operationCount");

if (arraySizeInput) {
    arraySizeInput.addEventListener("input", () => {
        if (arraySizeLabel) {
            arraySizeLabel.textContent = arraySizeInput.value;
        }

        generateArray();
    });
}

if (generateArrayButton) {
    generateArrayButton.addEventListener("click", () => {
        if (!isSorting) generateArray();
    });
}

if (startVisualizationButton) {
    startVisualizationButton.addEventListener("click", async () => {
        if (isSorting) return;
        await startSortingVisualization();
    });
}

function generateArray() {
    if (!arrayBars || isSorting) return;

    const size = Number(arraySizeInput?.value || 24);

    currentArray = Array.from(
        { length: size },
        () => Math.floor(Math.random() * 90) + 10
    );

    operationCounter = 0;
    updateOperationCount();
    renderArray();
    showVisualizerStatus("New array ready. Start visualization to sort it.");
}

function renderArray(highlightIndices = [], sortedIndices = []) {
    if (!arrayBars) return;

    arrayBars.replaceChildren();

    const maxValue = Math.max(...currentArray, 1);

    currentArray.forEach((value, index) => {
        const bar = document.createElement("div");
        bar.className = "array-bar";
        bar.style.height = `${(value / maxValue) * 90}%`;
        bar.title = `Value: ${value}`;

        if (sortedIndices.includes(index)) {
            bar.classList.add("sorted");
        } else if (highlightIndices.includes(index)) {
            bar.classList.add("comparing");
        }

        arrayBars.appendChild(bar);
    });
}

function updateOperationCount() {
    if (operationCountDisplay) {
        operationCountDisplay.textContent =
            formatNumber(operationCounter);
    }
}

function showVisualizerStatus(message, isError = false) {
    if (!visualizerStatus) return;

    visualizerStatus.textContent = message;
    visualizerStatus.style.color = isError ? "#fb7185" : "#91a0b9";
}

function sleep(milliseconds) {
    return new Promise((resolve) => setTimeout(resolve, milliseconds));
}

async function startSortingVisualization() {
    if (currentArray.length === 0) {
        generateArray();
    }

    if (currentArray.length === 0) return;

    isSorting = true;
    operationCounter = 0;
    updateOperationCount();

    if (startVisualizationButton) {
        startVisualizationButton.disabled = true;
        startVisualizationButton.textContent = "Sorting...";
    }

    if (generateArrayButton) {
        generateArrayButton.disabled = true;
    }

    if (arraySizeInput) {
        arraySizeInput.disabled = true;
    }

    if (sortAlgorithmSelect) {
        sortAlgorithmSelect.disabled = true;
    }

    try {
        const algorithm = sortAlgorithmSelect?.value || "bubble";

        showVisualizerStatus(
            `${capitalize(algorithm)} sort visualization running...`
        );

        if (algorithm === "bubble") {
            await bubbleSortAnimation();
        } else if (algorithm === "insertion") {
            await insertionSortAnimation();
        } else if (algorithm === "merge") {
            await mergeSortAnimation();
        } else {
            throw new Error("Unsupported sorting algorithm.");
        }

        renderArray([], currentArray.map((_, index) => index));

        showVisualizerStatus(
            `Sorting complete! ${capitalize(algorithm)} sort finished.`
        );

    } catch (error) {
        console.error("Visualizer error:", error);
        showVisualizerStatus(error.message || "Visualization failed.", true);

    } finally {
        isSorting = false;

        if (startVisualizationButton) {
            startVisualizationButton.disabled = false;
            startVisualizationButton.textContent = "▶ Start Sorting";
        }

        if (generateArrayButton) {
            generateArrayButton.disabled = false;
        }

        if (arraySizeInput) {
            arraySizeInput.disabled = false;
        }

        if (sortAlgorithmSelect) {
            sortAlgorithmSelect.disabled = false;
        }
    }
}

async function bubbleSortAnimation() {
    const length = currentArray.length;
    const delay = getAnimationDelay(length);

    for (let i = 0; i < length - 1; i++) {
        for (let j = 0; j < length - i - 1; j++) {
            operationCounter++;
            updateOperationCount();

            renderArray([j, j + 1]);
            await sleep(delay);

            if (currentArray[j] > currentArray[j + 1]) {
                [currentArray[j], currentArray[j + 1]] =
                    [currentArray[j + 1], currentArray[j]];
            }
        }

        renderArray([], currentArray.map((_, index) =>
            index >= length - i - 1 ? index : -1
        ).filter((index) => index >= 0));
    }
}

async function insertionSortAnimation() {
    const length = currentArray.length;
    const delay = getAnimationDelay(length);

    for (let i = 1; i < length; i++) {
        const key = currentArray[i];
        let j = i - 1;

        while (j >= 0) {
            operationCounter++;
            updateOperationCount();

            renderArray([j, j + 1]);
            await sleep(delay);

            if (currentArray[j] > key) {
                currentArray[j + 1] = currentArray[j];
                j--;
            } else {
                break;
            }
        }

        currentArray[j + 1] = key;
        renderArray();
    }
}

async function mergeSortAnimation() {
    const delay = getAnimationDelay(currentArray.length);

    async function mergeSort(left, right) {
        if (left >= right) return;

        const middle = Math.floor((left + right) / 2);

        await mergeSort(left, middle);
        await mergeSort(middle + 1, right);

        const leftPart = currentArray.slice(left, middle + 1);
        const rightPart = currentArray.slice(middle + 1, right + 1);

        let i = 0;
        let j = 0;
        let k = left;

        while (i < leftPart.length && j < rightPart.length) {
            operationCounter++;
            updateOperationCount();

            renderArray([k]);
            await sleep(delay);

            if (leftPart[i] <= rightPart[j]) {
                currentArray[k] = leftPart[i++];
            } else {
                currentArray[k] = rightPart[j++];
            }

            k++;
        }

        while (i < leftPart.length) {
            currentArray[k++] = leftPart[i++];
        }

        while (j < rightPart.length) {
            currentArray[k++] = rightPart[j++];
        }

        renderArray();
    }

    await mergeSort(0, currentArray.length - 1);
}

function getAnimationDelay(size) {
    if (size >= 40) return 8;
    if (size >= 28) return 18;
    if (size >= 18) return 30;
    return 45;
}

/* ==========================================
   UTILITIES
   ========================================== */

function formatNumber(value) {
    const number = Number(value);

    if (!Number.isFinite(number)) return String(value ?? "—");

    return new Intl.NumberFormat("en-IN", {
        maximumFractionDigits: 0
    }).format(number);
}

function formatTime(value) {
    const number = Number(value);

    return Number.isFinite(number)
        ? number.toFixed(3)
        : "—";
}

function capitalize(value) {
    if (!value) return "";
    return value.charAt(0).toUpperCase() + value.slice(1);
}

/* ==========================================
   INITIALIZE
   ========================================== */

generateArray();

console.log("AlgoSentinel frontend initialized.");