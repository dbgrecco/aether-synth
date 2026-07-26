/*
 * Aether Synth Official Web Application Script
 * Real-time 3D Wavetable Canvas, QWERTY Musical Keyboard & Mouse XY Mod/Pitch Pad
 */

document.addEventListener('DOMContentLoaded', () => {
    init3DHeroVisualizer();
    initWebAudioSynth();
});

/* ==========================================
   1. Real-Time 3D Wavetable & Mouse XY Mod Pad
   ========================================== */
let globalMousePitchBend = 1.0; // Pitch Bend multiplier
let globalMouseModWheel = 0.5;   // Mod Wheel (0.0 to 1.0)

function init3DHeroVisualizer() {
    const canvas = document.getElementById('hero-canvas');
    if (!canvas) return;

    const ctx = canvas.getContext('2d');
    let width = 0;
    let height = 0;

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

    // Mouse XY Pad Modulation Listener
    canvas.addEventListener('mousemove', (e) => {
        const rect = canvas.getBoundingClientRect();
        const mouseX = e.clientX - rect.left;
        const mouseY = e.clientY - rect.top;

        // X Axis: Pitch Bend (0.5x to 2.0x pitch shift)
        const normX = (mouseX / rect.width);
        globalMousePitchBend = Math.pow(2.0, (normX - 0.5) * 2.0); // +/- 12 semitones

        // Y Axis: Mod Wheel / Filter Cutoff (0.0 to 1.0)
        const normY = 1.0 - (mouseY / rect.height);
        globalMouseModWheel = Math.max(0.01, Math.min(1.0, normY));

        // Update cutoff slider dynamically if Web Audio Filter exists
        const cutoffSlider = document.getElementById('cutoff-slider');
        if (cutoffSlider) {
            const newCutoff = 100 + globalMouseModWheel * 7900;
            cutoffSlider.value = newCutoff;
            cutoffSlider.dispatchEvent(new Event('input'));
        }
    });

    canvas.addEventListener('mouseleave', () => {
        globalMousePitchBend = 1.0; // Reset pitch bend on leave
    });

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
                
                // Modulated wave distortion using Mouse XY ModWheel
                let yNorm = Math.sin(phase * frameHarmonic * globalMousePitchBend + time * 2) * (1 / (1 + f * 0.15))
                           + Math.sin(phase * 3 * globalMouseModWheel - time) * 0.25;

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

        requestAnimationFrame(render);
    }

    render();
}

/* ==========================================
   2. Interactive Web Audio & QWERTY Keyboard
   ========================================== */
function initWebAudioSynth() {
    let audioCtx = null;
    let mainFilter = null;
    let analyser = null;
    let activeOscillators = {};

    const presetSelect = document.getElementById('preset-select');
    const cutoffSlider = document.getElementById('cutoff-slider');
    const keys = document.querySelectorAll('.piano-keyboard .key');
    const scopeCanvas = document.getElementById('synth-scope-canvas');

    // QWERTY Key Map to Frequencies (Base Octave 4)
    let octaveShift = 0; // Z / X to shift octaves
    const qwertyNoteMap = {
        'a': 261.63, // C4
        'w': 277.18, // C#4
        's': 293.66, // D4
        'e': 311.13, // D#4
        'd': 329.63, // E4
        'f': 349.23, // F4
        't': 369.99, // F#4
        'g': 392.00, // G4
        'y': 415.30, // G#4
        'h': 440.00, // A4
        'u': 466.16, // A#4
        'j': 493.88, // B4
        'k': 523.25, // C5
        'o': 554.37, // C#5
        'l': 587.33  // D5
    };

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

    function triggerNoteStart(freq, keyChar = null) {
        ensureAudioContext();

        const actualFreq = freq * Math.pow(2, octaveShift) * globalMousePitchBend;
        const preset = presetSelect ? presetSelect.value : 'cyberpunk';
        const now = audioCtx.currentTime;

        // If key already playing, don't duplicate
        if (keyChar && activeOscillators[keyChar]) return;

        const osc1 = audioCtx.createOscillator();
        const osc2 = audioCtx.createOscillator();
        const noteGain = audioCtx.createGain();

        switch (preset) {
            case 'cyberpunk':
                osc1.type = 'sawtooth';
                osc2.type = 'square';
                osc1.frequency.value = actualFreq;
                osc2.frequency.value = actualFreq * 1.005;
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.3, now + 0.02);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 1.2);
                break;

            case 'shimmer':
                osc1.type = 'sine';
                osc2.type = 'triangle';
                osc1.frequency.value = actualFreq;
                osc2.frequency.value = actualFreq * 2.0;
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.25, now + 0.15);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 2.5);
                break;

            case 'sub':
                osc1.type = 'sawtooth';
                osc2.type = 'sine';
                osc1.frequency.value = actualFreq * 0.5;
                osc2.frequency.value = actualFreq * 0.5;
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.4, now + 0.01);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 0.8);
                break;

            case 'granular':
                osc1.type = 'triangle';
                osc2.type = 'sine';
                osc1.frequency.value = actualFreq;
                osc2.frequency.value = actualFreq * 3.01;
                noteGain.gain.setValueAtTime(0.001, now);
                noteGain.gain.linearRampToValueAtTime(0.25, now + 0.05);
                noteGain.gain.exponentialRampToValueAtTime(0.001, now + 1.5);
                break;
        }

        osc1.connect(noteGain);
        osc2.connect(noteGain);
        noteGain.connect(mainFilter);

        osc1.start(now);
        osc2.start(now);

        if (keyChar) {
            activeOscillators[keyChar] = { osc1, osc2, noteGain };
        } else {
            osc1.stop(now + 2.1);
            osc2.stop(now + 2.1);
        }
    }

    function triggerNoteEnd(keyChar) {
        if (activeOscillators[keyChar]) {
            const { osc1, osc2, noteGain } = activeOscillators[keyChar];
            const now = audioCtx.currentTime;
            noteGain.gain.cancelScheduledValues(now);
            noteGain.gain.exponentialRampToValueAtTime(0.0001, now + 0.1);
            osc1.stop(now + 0.12);
            osc2.stop(now + 0.12);
            delete activeOscillators[keyChar];
        }
    }

    // Attach QWERTY Physical Keyboard Listeners
    window.addEventListener('keydown', (e) => {
        if (e.target.tagName === 'INPUT' || e.target.tagName === 'SELECT') return;

        const key = e.key.toLowerCase();
        if (key === 'z') {
            octaveShift = Math.max(-2, octaveShift - 1);
            showKeyboardToast(`Oitava: ${octaveShift >= 0 ? '+' : ''}${octaveShift}`);
        } else if (key === 'x') {
            octaveShift = Math.min(2, octaveShift + 1);
            showKeyboardToast(`Oitava: ${octaveShift >= 0 ? '+' : ''}${octaveShift}`);
        } else if (qwertyNoteMap[key]) {
            highlightPianoKey(qwertyNoteMap[key], true);
            triggerNoteStart(qwertyNoteMap[key], key);
        }
    });

    window.addEventListener('keyup', (e) => {
        const key = e.key.toLowerCase();
        if (qwertyNoteMap[key]) {
            highlightPianoKey(qwertyNoteMap[key], false);
            triggerNoteEnd(key);
        }
    });

    // Helper to highlight corresponding UI key
    function highlightPianoKey(freq, isActive) {
        keys.forEach((key) => {
            if (Math.abs(parseFloat(key.getAttribute('data-note')) - freq) < 1.0) {
                if (isActive) key.classList.add('active');
                else key.classList.remove('active');
            }
        });
    }

    // Mouse / Touch Piano Keys
    keys.forEach((key) => {
        const freq = parseFloat(key.getAttribute('data-note'));

        key.addEventListener('mousedown', () => {
            key.classList.add('active');
            triggerNoteStart(freq);
        });

        key.addEventListener('mouseup', () => key.classList.remove('active'));
        key.addEventListener('mouseleave', () => key.classList.remove('active'));
    });
}

function showKeyboardToast(msg) {
    let toast = document.getElementById('qwerty-toast');
    if (!toast) {
        toast = document.createElement('div');
        toast.id = 'qwerty-toast';
        toast.style.cssText = 'position:fixed;bottom:24px;right:24px;background:#00E5FF;color:#090B10;font-weight:800;padding:8px 16px;border-radius:20px;z-index:9999;box-shadow:0 0 15px rgba(0,229,255,0.5);font-family:sans-serif;font-size:0.85rem;';
        document.body.appendChild(toast);
    }
    toast.textContent = msg;
    toast.style.opacity = '1';
    setTimeout(() => { toast.style.opacity = '0'; }, 1500);
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
