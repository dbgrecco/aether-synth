/*
 * Aether Synth Official Web Application Script
 * Real-time 3D Wavetable Canvas & Interactive Web Audio Synthesizer Engine
 */

document.addEventListener('DOMContentLoaded', () => {
    init3DHeroVisualizer();
    initWebAudioSynth();
});

/* ==========================================
   1. Real-Time 3D Wavetable Hero Visualizer
   ========================================== */
function init3DHeroVisualizer() {
    const canvas = document.getElementById('hero-canvas');
    if (!canvas) return;

    const ctx = canvas.getContext('2d');
    let width = 0;
    let height = 0;
    let animFrameId = null;

    function resize() {
        const rect = canvas.parentElement.getBoundingClientRect();
        const dpr = window.devicePixelRatio || 1;
        width = rect.width;
        height = rect.height;
        canvas.width = width * dpr;
        canvas.height = height * dpr;
        ctx.scale(dpr, dpr);
    }

    window.addEventListener('resize', resize);
    resize();

    let time = 0;

    function render() {
        time += 0.015;
        ctx.clearRect(0, 0, width, height);

        const numFrames = 22;
        const numSamples = 80;
        const cx = width / 2;
        const cy = height / 2 + 30;
        const scaleX = width * 0.38;
        const scaleY = height * 0.28;

        const pitch = 0.55;
        const yaw = Math.sin(time * 0.3) * 0.4;

        const cosYaw = Math.cos(yaw);
        const sinYaw = Math.sin(yaw);
        const sinPitch = Math.sin(pitch);

        const highlightIdx = Math.floor((Math.sin(time * 0.8) * 0.5 + 0.5) * (numFrames - 1));

        for (let f = numFrames - 1; f >= 0; f--) {
            const zNorm = (f / (numFrames - 1)) - 0.5;
            const isHighlight = (f === highlightIdx);

            ctx.beginPath();
            let firstPoint = true;

            const frameHarmonic = 1.0 + f * 0.45;

            for (let i = 0; i < numSamples; i++) {
                const xNorm = (i / (numSamples - 1)) - 0.5;
                const phase = (i / numSamples) * Math.PI * 2;
                
                // Dynamic wavetable morphing formula
                let yNorm = Math.sin(phase * frameHarmonic + time * 2) * (1 / (1 + f * 0.15))
                           + Math.sin(phase * 3 - time) * 0.25;

                // 3D projection
                const x3d = xNorm * cosYaw - zNorm * sinYaw;
                const z3d = xNorm * sinYaw + zNorm * cosYaw;
                const y3d = yNorm * 0.45 - z3d * sinPitch;

                const screenX = cx + x3d * scaleX;
                const screenY = cy - y3d * scaleY;

                if (firstPoint) {
                    ctx.moveTo(screenX, screenY);
                    firstPoint = false;
                } else {
                    ctx.lineTo(screenX, screenY);
                }
            }

            ctx.lineWidth = isHighlight ? 2.8 : 1.2;
            if (isHighlight) {
                ctx.strokeStyle = '#00E5FF';
                ctx.shadowColor = '#00E5FF';
                ctx.shadowBlur = 15;
            } else {
                ctx.shadowBlur = 0;
                const hue = 210 + f * 4;
                const alpha = 0.35 + (f / numFrames) * 0.5;
                ctx.strokeStyle = `hsla(${hue}, 85%, 65%, ${alpha})`;
            }

            ctx.stroke();
            ctx.shadowBlur = 0;
        }

        animFrameId = requestAnimationFrame(render);
    }

    render();
}

/* ==========================================
   2. Interactive Web Audio Synthesizer Engine
   ========================================== */
function initWebAudioSynth() {
    let audioCtx = null;
    let mainFilter = null;
    let reverbGain = null;
    let analyser = null;

    const presetSelect = document.getElementById('preset-select');
    const cutoffSlider = document.getElementById('cutoff-slider');
    const reverbSlider = document.getElementById('reverb-slider');
    const keys = document.querySelectorAll('.piano-keyboard .key');
    const scopeCanvas = document.getElementById('synth-scope-canvas');

    function ensureAudioContext() {
        if (!audioCtx) {
            audioCtx = new (window.AudioContext || window.webkitAudioContext)();
            
            mainFilter = audioCtx.createBiquadFilter();
            mainFilter.type = 'lowpass';
            mainFilter.frequency.value = parseFloat(cutoffSlider.value);

            analyser = audioCtx.createAnalyser();
            analyser.fftSize = 2048;

            mainFilter.connect(analyser);
            analyser.connect(audioCtx.destination);

            startOscilloscope(scopeCanvas, analyser);
        }
        if (audioCtx.state === 'suspended') {
            audioCtx.resume();
        }
    }

    if (cutoffSlider) {
        cutoffSlider.addEventListener('input', (e) => {
            if (mainFilter) {
                mainFilter.frequency.setTargetAtTime(parseFloat(e.target.value), audioCtx.currentTime, 0.05);
            }
        });
    }

    // Play Synth Note
    function playNote(freq) {
        ensureAudioContext();

        const preset = presetSelect ? presetSelect.value : 'cyberpunk';
        const now = audioCtx.currentTime;

        const osc1 = audioCtx.createOscillator();
        const osc2 = audioCtx.createOscillator();
        const noteGain = audioCtx.createGain();

        // Preset sound configuration
        switch (preset) {
            case 'cyberpunk':
                osc1.type = 'sawtooth';
                osc2.type = 'square';
                osc1.frequency.value = freq;
                osc2.frequency.value = freq * 1.005; // Detuned
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.3, now + 0.02);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 0.8);
                break;

            case 'shimmer':
                osc1.type = 'sine';
                osc2.type = 'triangle';
                osc1.frequency.value = freq;
                osc2.frequency.value = freq * 2.0; // Octave up shimmer
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.25, now + 0.2);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 2.0);
                break;

            case 'sub':
                osc1.type = 'sawtooth';
                osc2.type = 'sine';
                osc1.frequency.value = freq * 0.5;
                osc2.frequency.value = freq * 0.5;
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.4, now + 0.01);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 0.5);
                break;

            case 'granular':
                osc1.type = 'triangle';
                osc2.type = 'sine';
                osc1.frequency.value = freq;
                osc2.frequency.value = freq * 3.01;
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.25, now + 0.05);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 1.2);
                break;
        }

        osc1.connect(noteGain);
        osc2.connect(noteGain);
        noteGain.connect(mainFilter);

        osc1.start(now);
        osc2.start(now);
        osc1.stop(now + 2.1);
        osc2.stop(now + 2.1);
    }

    // Attach piano key triggers
    keys.forEach((key) => {
        const freq = parseFloat(key.getAttribute('data-note'));

        key.addEventListener('mousedown', () => {
            key.classList.add('active');
            playNote(freq);
        });

        key.addEventListener('mouseup', () => key.classList.remove('active'));
        key.addEventListener('mouseleave', () => key.classList.remove('active'));

        key.addEventListener('touchstart', (e) => {
            e.preventDefault();
            key.classList.add('active');
            playNote(freq);
        });

        key.addEventListener('touchend', () => key.classList.remove('active'));
    });
}

/* Oscilloscope Canvas Visualizer */
function startOscilloscope(canvas, analyser) {
    if (!canvas || !analyser) return;

    const ctx = canvas.getContext('2d');
    const bufferLength = analyser.frequencyBinCount;
    const dataArray = new Uint8Array(bufferLength);

    function draw() {
        requestAnimationFrame(draw);

        const dpr = window.devicePixelRatio || 1;
        const width = canvas.clientWidth;
        const height = canvas.clientHeight;
        canvas.width = width * dpr;
        canvas.height = height * dpr;
        ctx.scale(dpr, dpr);

        analyser.getByteTimeDomainData(dataArray);

        ctx.fillStyle = 'rgba(10, 13, 20, 0.4)';
        ctx.fillRect(0, 0, width, height);

        ctx.lineWidth = 2;
        ctx.strokeStyle = '#00E5FF';
        ctx.beginPath();

        const sliceWidth = width / bufferLength;
        let x = 0;

        for (let i = 0; i < bufferLength; i++) {
            const v = dataArray[i] / 128.0;
            const y = (v * height) / 2;

            if (i === 0) {
                ctx.moveTo(x, y);
            } else {
                ctx.lineTo(x, y);
            }

            x += sliceWidth;
        }

        ctx.stroke();
    }

    draw();
}
