package watchdog

// fra chatgpt-pavel

import (
    "time"
)

type Watchdog struct {
    interval time.Duration
    stopCh   chan bool
    eventCh  chan bool
    ticker   *time.Ticker
}

func New(interval time.Duration) *Watchdog {
    return &Watchdog{
        interval: interval,
        stopCh:   make(chan bool),
        eventCh:  make(chan bool),
        ticker:   nil,
    }
}

func (w *Watchdog) Start() {
    w.ticker = time.NewTicker(w.interval)
    defer w.ticker.Stop()

    for {
        select {
        case <-w.ticker.C:
            // Check for events or conditions here
            // If an event is detected, send it to the event channel
            w.eventCh <- true
        case <-w.stopCh:
            return
        }
    }
}

func (w *Watchdog) Stop() {
    w.stopCh <- true
}

func (w *Watchdog) Event() <-chan bool {
    return w.eventCh
}

func (w *Watchdog) Reset(interval time.Duration) {
    w.interval = interval
    if w.ticker != nil {
        w.ticker.Stop()
    }
    w.ticker = time.NewTicker(w.interval)
}

