from playwright.sync_api import sync_playwright

def run(playwright):
    browser = playwright.chromium.launch()
    page = browser.new_page()
    page.goto("http://192.168.4.1/control.html")
    page.screenshot(path="jules-scratch/verification/control_page.png")
    browser.close()

with sync_playwright() as playwright:
    run(playwright)