/*
 * Aether Synth Official Web Application Script
 * Real-time 3D Wavetable Canvas, QWERTY Musical Keyboard & Korg Vector Joystick Controller
 */

document.addEventListener('DOMContentLoaded', () => {
    init3DHeroVisualizer();
    initParticleField();
    initWebAudioSynth();
    initKorgJoystick();
    initStatsCounters();
    initPresetCards();
});

/* ==========================================
   1. Real-Time 3D Wavetable Hero Visualizer
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
   2. Korg Vector Joystick Controller
   ========================================== */
function initKorgJoystick() {
    const pad = document.getElementById('joystick-pad');
    const handle = document.getElementById('joystick-handle');
    const pitchReadout = document.getElementById('joy-pitch-readout');
    const modReadout = document.getElementById('joy-mod-readout');

    if (!pad || !handle) return;

    let isDragging = false;
    let currentX = 0; // -1 to +1 (Pitch Bend)
    let currentY = 0; // -1 to +1 (Mod Wheel / Cutoff)

    function updateJoystickPosition(clientX, clientY) {
        const rect = pad.getBoundingClientRect();
        const radius = rect.width / 2;
        const centerX = rect.left + radius;
        const centerY = rect.top + radius;

        let deltaX = clientX - centerX;
        let deltaY = clientY - centerY;

        // Limit to circle radius
        const dist = Math.sqrt(deltaX * deltaX + deltaY * deltaY);
        if (dist > radius - 10) {
            deltaX = (deltaX / dist) * (radius - 10);
            deltaY = (deltaY / dist) * (radius - 10);
        }

        currentX = deltaX / (radius - 10); // -1.0 to +1.0
        currentY = -deltaY / (radius - 10); // -1.0 to +1.0 (inverted for screen Y)

        // Move handle
        handle.style.transform = `translate(${deltaX}px, ${deltaY}px)`;

        // Calculate Pitch Bend (-12 to +12 semitones)
        const semitones = (currentX * 12.0).toFixed(1);
        globalMousePitchBend = Math.pow(2.0, currentX);

        // Calculate Mod Wheel (0% to 100%)
        const modPercent = Math.round(((currentY + 1.0) / 2.0) * 100);
        globalMouseModWheel = (currentY + 1.0) / 2.0;

        // Update readouts
        if (pitchReadout) pitchReadout.textContent = `Pitch: ${semitones > 0 ? '+' : ''}${semitones}st`;
        if (modReadout) modReadout.textContent = `Mod: ${modPercent}%`;

        // Update filter cutoff slider dynamically
        const cutoffSlider = document.getElementById('cutoff-slider');
        if (cutoffSlider) {
            const newCutoff = 100 + globalMouseModWheel * 7900;
            cutoffSlider.value = newCutoff;
            cutoffSlider.dispatchEvent(new Event('input'));
        }
    }

    function resetJoystickSpring() {
        // X-axis springs back to center (0.0 pitch bend), Y-axis stays where set
        currentX = 0;
        const rect = pad.getBoundingClientRect();
        const radius = rect.width / 2;
        const deltaY = -currentY * (radius - 10);

        handle.style.transform = `translate(0px, ${deltaY}px)`;
        globalMousePitchBend = 1.0;

        if (pitchReadout) pitchReadout.textContent = `Pitch: 0.0st`;
    }

    pad.addEventListener('mousedown', (e) => {
        isDragging = true;
        updateJoystickPosition(e.clientX, e.clientY);
    });

    window.addEventListener('mousemove', (e) => {
        if (isDragging) {
            updateJoystickPosition(e.clientX, e.clientY);
        }
    });

    window.addEventListener('mouseup', () => {
        if (isDragging) {
            isDragging = false;
            resetJoystickSpring();
        }
    });

    // Touch support for mobile/tablets
    pad.addEventListener('touchstart', (e) => {
        if (e.touches.length > 0) {
            isDragging = true;
            updateJoystickPosition(e.touches[0].clientX, e.touches[0].clientY);
        }
    });

    window.addEventListener('touchmove', (e) => {
        if (isDragging && e.touches.length > 0) {
            updateJoystickPosition(e.touches[0].clientX, e.touches[0].clientY);
        }
    });

    window.addEventListener('touchend', () => {
        if (isDragging) {
            isDragging = false;
            resetJoystickSpring();
        }
    });
}

/* ==========================================
   3. Interactive Web Audio & QWERTY Keyboard
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

    let octaveShift = 0;
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
            mainFilter.frequency.value = parseFloat(cutoffSlider ? cutoffSlider.value : 2500);

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

    if (nebulaDriftSlider) {
        nebulaDriftSlider.addEventListener('input', (e) => {
            nebulaDriftAmount = parseFloat(e.target.value) / 100.0;
        });
    }

    if (nebulaSatSlider) {
        nebulaSatSlider.addEventListener('input', (e) => {
            nebulaSatAmount = parseFloat(e.target.value) / 100.0;
        });
    }

    function triggerNoteStart(freq, keyChar = null) {
        ensureAudioContext();

        const actualFreq = freq * Math.pow(2, octaveShift) * globalMousePitchBend;
        const preset = presetSelect ? presetSelect.value : 'cyberpunk';
        const now = audioCtx.currentTime;

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

    function highlightPianoKey(freq, isActive) {
        keys.forEach((key) => {
            if (Math.abs(parseFloat(key.getAttribute('data-note')) - freq) < 1.0) {
                if (isActive) key.classList.add('active');
                else key.classList.remove('active');
            }
        });
    }

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

/* ==========================================
   4. Screenshot Gallery Controller
   ========================================== */
function initScreenshotGallery() {
    const frames = document.querySelectorAll('.screenshot-frame');
    const tabs   = document.querySelectorAll('.scr-tab');
    const dots   = document.querySelectorAll('.scr-dot');
    const prev   = document.getElementById('scr-prev');
    const next   = document.getElementById('scr-next');

    if (!frames.length) return;

    let current = 0;
    let autoTimer = null;

    function goTo(idx) {
        frames[current].classList.remove('active');
        tabs[current].classList.remove('active');
        dots[current].classList.remove('active');
        current = (idx + frames.length) % frames.length;
        frames[current].classList.add('active');
        tabs[current].classList.add('active');
        dots[current].classList.add('active');
    }

    function startAuto() {
        clearInterval(autoTimer);
        autoTimer = setInterval(() => goTo(current + 1), 6000);
    }

    function resetAuto() {
        startAuto();
    }

    tabs.forEach((tab, i) => {
        tab.addEventListener('click', () => { goTo(i); resetAuto(); });
    });

    dots.forEach((dot, i) => {
        dot.addEventListener('click', () => { goTo(i); resetAuto(); });
    });

    if (prev) prev.addEventListener('click', () => { goTo(current - 1); resetAuto(); });
    if (next) next.addEventListener('click', () => { goTo(current + 1); resetAuto(); });

    // Keyboard navigation when hovering the gallery
    document.getElementById('screenshot-viewer')?.addEventListener('keydown', (e) => {
        if (e.key === 'ArrowLeft')  { goTo(current - 1); resetAuto(); }
        if (e.key === 'ArrowRight') { goTo(current + 1); resetAuto(); }
    });

    startAuto();
}

document.addEventListener('DOMContentLoaded', () => {
    initScreenshotGallery();
});

/* ==========================================
   5. Hero Particle Field
   ========================================== */
function initParticleField() {
    const canvas = document.getElementById('particle-canvas');
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    let W = 0, H = 0, mouse = { x: -999, y: -999 };

    function resize() {
        const hero = canvas.closest('.hero-section');
        W = canvas.width  = hero ? hero.offsetWidth  : window.innerWidth;
        H = canvas.height = hero ? hero.offsetHeight : 700;
    }
    window.addEventListener('resize', resize);
    resize();

    // Track mouse relative to canvas
    canvas.closest('.hero-section')?.addEventListener('mousemove', (e) => {
        const r = canvas.getBoundingClientRect();
        mouse.x = e.clientX - r.left;
        mouse.y = e.clientY - r.top;
    });
    canvas.closest('.hero-section')?.addEventListener('mouseleave', () => {
        mouse.x = -999; mouse.y = -999;
    });

    const CYAN   = [0, 229, 255];
    const PURPLE = [124, 77, 255];
    const COUNT  = 80;

    const particles = Array.from({ length: COUNT }, () => ({
        x: Math.random() * W,
        y: Math.random() * H,
        vx: (Math.random() - 0.5) * 0.35,
        vy: (Math.random() - 0.5) * 0.35,
        r: Math.random() * 1.8 + 0.6,
        hue: Math.random() < 0.6 ? CYAN : PURPLE,
    }));

    const MAX_DIST  = 130;
    const MAX_MOUSE = 160;

    function lerp(a, b, t) { return a + (b - a) * t; }

    function draw() {
        ctx.clearRect(0, 0, W, H);

        // Draw connections
        for (let i = 0; i < particles.length; i++) {
            const p = particles[i];
            // Mouse attraction
            const mdx = mouse.x - p.x, mdy = mouse.y - p.y;
            const md  = Math.sqrt(mdx * mdx + mdy * mdy);
            if (md < MAX_MOUSE) {
                p.vx += mdx / md * 0.018;
                p.vy += mdy / md * 0.018;
            }
            // Damping
            p.vx *= 0.98; p.vy *= 0.98;
            p.x  += p.vx; p.y  += p.vy;
            // Wrap
            if (p.x < -10) p.x = W + 10;
            if (p.x > W + 10) p.x = -10;
            if (p.y < -10) p.y = H + 10;
            if (p.y > H + 10) p.y = -10;

            for (let j = i + 1; j < particles.length; j++) {
                const q   = particles[j];
                const dx  = p.x - q.x, dy = p.y - q.y;
                const dist = Math.sqrt(dx * dx + dy * dy);
                if (dist < MAX_DIST) {
                    const alpha = (1 - dist / MAX_DIST) * 0.45;
                    const [r1,g1,b1] = p.hue, [r2,g2,b2] = q.hue;
                    const t  = 0.5;
                    const cr = Math.round(lerp(r1,r2,t)), cg = Math.round(lerp(g1,g2,t)), cb = Math.round(lerp(b1,b2,t));
                    ctx.beginPath();
                    ctx.strokeStyle = `rgba(${cr},${cg},${cb},${alpha})`;
                    ctx.lineWidth = 0.8;
                    ctx.moveTo(p.x, p.y);
                    ctx.lineTo(q.x, q.y);
                    ctx.stroke();
                }
            }
        }

        // Draw dots
        for (const p of particles) {
            const [r,g,b] = p.hue;
            ctx.beginPath();
            ctx.arc(p.x, p.y, p.r, 0, Math.PI * 2);
            ctx.fillStyle = `rgba(${r},${g},${b},0.85)`;
            ctx.shadowColor = `rgb(${r},${g},${b})`;
            ctx.shadowBlur = 6;
            ctx.fill();
            ctx.shadowBlur = 0;
        }

        requestAnimationFrame(draw);
    }
    draw();
}

/* ==========================================
   6. Stats Animated Counters
   ========================================== */
function initStatsCounters() {
    const numbers = document.querySelectorAll('.stat-number');
    if (!numbers.length) return;

    function animateCounter(el) {
        const target = parseInt(el.dataset.target, 10);
        const suffix = el.dataset.suffix || '';
        const duration = 1600;
        const start = performance.now();

        function ease(t) { return t < 0.5 ? 4 * t * t * t : 1 - Math.pow(-2 * t + 2, 3) / 2; }

        function step(now) {
            const elapsed = now - start;
            const progress = Math.min(elapsed / duration, 1);
            const value = Math.round(ease(progress) * target);
            el.textContent = value + suffix;
            if (progress < 1) requestAnimationFrame(step);
        }
        requestAnimationFrame(step);
    }

    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.querySelectorAll('.stat-number').forEach(animateCounter);
                observer.unobserve(entry.target);
            }
        });
    }, { threshold: 0.4 });

    const section = document.getElementById('stats');
    if (section) observer.observe(section);
}

/* ==========================================
   7. Preset Cards & Auto-Demo
   ========================================== */
function initPresetCards() {
    const cards    = document.querySelectorAll('.preset-card');
    const playBtns = document.querySelectorAll('.preset-play-btn');
    const label    = document.getElementById('active-preset-label');
    const hiddenSelect = document.getElementById('preset-select');

    if (!cards.length) return;

    const PRESET_NAMES = {
        cyberpunk: 'Cyberpunk Saw',
        shimmer:   'Ambient Shimmer',
        sub:       'Deep Acid Sub',
        granular:  'Granular Bell',
        pluck:     'Crystal Pluck',
        'pad-dark': 'Dark Matter',
    };

    // Musical demo phrases (intervals in semitones from root, timing in seconds)
    const DEMOS = {
        cyberpunk:  { root: 220, notes: [0,7,12,7,0,5,7,12],    gap: 0.16, dur: 0.22 },
        shimmer:    { root: 261.63, notes: [0,4,7,12,7,4,0,-5], gap: 0.35, dur: 1.6  },
        sub:        { root: 110, notes: [0,0,12,0,0,7,0,5],     gap: 0.18, dur: 0.25 },
        granular:   { root: 329.63, notes: [0,3,7,10,12,10,7,3],gap: 0.28, dur: 0.8  },
        pluck:      { root: 392, notes: [0,5,9,12,16,12,9,5],   gap: 0.13, dur: 0.18 },
        'pad-dark': { root: 146.83, notes: [0,3,7,10,12],       gap: 0.5,  dur: 2.2  },
    };

    let audioCtx = null;
    let demoTimeout = null;

    function getAudioCtx() {
        if (!audioCtx) audioCtx = new (window.AudioContext || window.webkitAudioContext)();
        if (audioCtx.state === 'suspended') audioCtx.resume();
        return audioCtx;
    }

    function semitoneToFreq(root, semitones) {
        return root * Math.pow(2, semitones / 12);
    }

    function playDemoNote(ctx, freq, preset, startTime, duration) {
        const osc1 = ctx.createOscillator();
        const osc2 = ctx.createOscillator();
        const gain = ctx.createGain();
        const filter = ctx.createBiquadFilter();
        filter.type = 'lowpass';
        filter.frequency.value = 3000;

        switch (preset) {
            case 'cyberpunk':
                osc1.type = 'sawtooth'; osc2.type = 'square';
                osc1.frequency.value = freq; osc2.frequency.value = freq * 1.007;
                gain.gain.setValueAtTime(0.001, startTime);
                gain.gain.linearRampToValueAtTime(0.22, startTime + 0.02);
                gain.gain.exponentialRampToValueAtTime(0.001, startTime + duration);
                break;
            case 'shimmer':
                osc1.type = 'sine'; osc2.type = 'triangle';
                osc1.frequency.value = freq; osc2.frequency.value = freq * 2;
                gain.gain.setValueAtTime(0.001, startTime);
                gain.gain.linearRampToValueAtTime(0.18, startTime + 0.12);
                gain.gain.exponentialRampToValueAtTime(0.001, startTime + duration);
                break;
            case 'sub':
                osc1.type = 'sawtooth'; osc2.type = 'sine';
                osc1.frequency.value = freq * 0.5; osc2.frequency.value = freq * 0.5;
                gain.gain.setValueAtTime(0.001, startTime);
                gain.gain.linearRampToValueAtTime(0.32, startTime + 0.01);
                gain.gain.exponentialRampToValueAtTime(0.001, startTime + duration);
                break;
            case 'granular':
                osc1.type = 'triangle'; osc2.type = 'sine';
                osc1.frequency.value = freq; osc2.frequency.value = freq * 3.01;
                gain.gain.setValueAtTime(0.001, startTime);
                gain.gain.linearRampToValueAtTime(0.18, startTime + 0.06);
                gain.gain.exponentialRampToValueAtTime(0.001, startTime + duration);
                break;
            case 'pluck':
                osc1.type = 'sawtooth'; osc2.type = 'sine';
                osc1.frequency.value = freq; osc2.frequency.value = freq * 2.002;
                filter.frequency.setValueAtTime(6000, startTime);
                filter.frequency.exponentialRampToValueAtTime(400, startTime + duration);
                gain.gain.setValueAtTime(0.001, startTime);
                gain.gain.linearRampToValueAtTime(0.28, startTime + 0.005);
                gain.gain.exponentialRampToValueAtTime(0.001, startTime + duration);
                break;
            case 'pad-dark':
                osc1.type = 'sawtooth'; osc2.type = 'triangle';
                osc1.frequency.value = freq * 0.998; osc2.frequency.value = freq * 1.003;
                gain.gain.setValueAtTime(0.001, startTime);
                gain.gain.linearRampToValueAtTime(0.16, startTime + 0.3);
                gain.gain.exponentialRampToValueAtTime(0.001, startTime + duration);
                break;
        }

        osc1.connect(gain); osc2.connect(gain);
        gain.connect(filter); filter.connect(ctx.destination);
        osc1.start(startTime); osc2.start(startTime);
        osc1.stop(startTime + duration + 0.05);
        osc2.stop(startTime + duration + 0.05);
    }

    function playDemo(presetKey, btnEl) {
        const ctx = getAudioCtx();
        const demo = DEMOS[presetKey];
        if (!demo) return;

        // Stop any in-progress demo
        clearTimeout(demoTimeout);
        playBtns.forEach(b => { b.classList.remove('playing'); b.textContent = '▶ Demo'; });

        btnEl.classList.add('playing');
        btnEl.textContent = '⏸ Playing';

        const now = ctx.currentTime + 0.05;
        demo.notes.forEach((semitones, i) => {
            const freq = semitoneToFreq(demo.root, semitones);
            playDemoNote(ctx, freq, presetKey, now + i * demo.gap, demo.dur);
        });

        const totalTime = (demo.notes.length * demo.gap + demo.dur) * 1000 + 200;
        demoTimeout = setTimeout(() => {
            btnEl.classList.remove('playing');
            btnEl.textContent = '▶ Demo';
        }, totalTime);
    }

    function selectCard(presetKey) {
        cards.forEach(c => {
            const active = c.dataset.preset === presetKey;
            c.classList.toggle('active', active);
            c.setAttribute('aria-checked', active ? 'true' : 'false');
        });
        if (hiddenSelect) hiddenSelect.value = presetKey;
        if (label) label.textContent = PRESET_NAMES[presetKey] || presetKey;
        // Sync to existing synth engine
        hiddenSelect?.dispatchEvent(new Event('change'));
    }

    cards.forEach(card => {
        card.addEventListener('click', (e) => {
            if (e.target.classList.contains('preset-play-btn')) return;
            selectCard(card.dataset.preset);
        });
        card.addEventListener('keydown', (e) => {
            if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); selectCard(card.dataset.preset); }
        });
    });

    playBtns.forEach(btn => {
        btn.addEventListener('click', (e) => {
            e.stopPropagation();
            const preset = btn.dataset.preset;
            selectCard(preset);
            playDemo(preset, btn);
        });
    });
}
