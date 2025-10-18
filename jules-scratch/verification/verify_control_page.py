from playwright.sync_api import sync_playwright

with sync_playwright() as p:
    browser = p.chromium.launch()
    page = browser.new_page()
    page.goto("http://localhost:8000/control.html")
    page.screenshot(path="jules-scratch/verification/control_page.png")
    browser.close()