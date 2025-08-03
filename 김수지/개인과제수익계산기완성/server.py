import http.server
import socketserver
import subprocess
import json
from urllib.parse import parse_qs
import sys
import os

PORT = 8000

class MyHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html; charset=utf-8')
            self.end_headers()
            try:
                with open('index.html', 'rb') as file:
                    self.wfile.write(file.read())
            except FileNotFoundError:
                self.wfile.write("<html><body><h1>Error: index.html not found.</h1></body></html>".encode('utf-8'))
        else:
            self.send_error(404, "Page not found.")

    def do_POST(self):
        if self.path == '/calculate':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            params = parse_qs(post_data.decode('utf-8'))

            total_months = params.get('total_months', [''])[0]
            initial_capital = params.get('initial_capital', [''])[0]
            monthly_contribution = params.get('monthly_contribution', [''])[0]
            target_profit = params.get('target_profit', [''])[0]
            include_first_month = params.get('include_first_month', ['false'])[0]
            
            include_first_month_flag = "1" if include_first_month == 'true' else "0"

            try:
                result = subprocess.run(
                    ['./calculator', total_months, initial_capital, monthly_contribution, target_profit, include_first_month_flag],
                    capture_output=True, text=True, check=True, encoding='utf-8'
                )
                
                json_data = json.loads(result.stdout)
                
                table_rows = ""
                total_profit_sum = 0.0
                if 'monthly_data' in json_data:
                    total_profit_sum = json_data.get('total_profit_sum', 0.0)
                    for row in json_data['monthly_data']:
                        table_rows += f"""
                        <tr>
                            <td>{row['month']}</td>
                            <td>{row['capital']:.2f} 만 원</td>
                            <td>{row['profit']:.2f} 만 원</td>
                        </tr>
                        """
                
                table_rows += f"""
                <tr style="font-weight: bold;">
                    <td>합계</td>
                    <td></td>
                    <td>{total_profit_sum:.2f} 만 원</td>
                </tr>
                """

                response_html = f"""
                <!DOCTYPE html>
                <html lang="ko">
                <head>
                    <meta charset="UTF-8">
                    <title>계산 결과</title>
                    <style>
                        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&family=Noto+Sans+KR:wght@400;500;700&display=swap');
                        :root {{
                            --primary-color: #56a3f5;
                            --primary-gradient: linear-gradient(45deg, #56a3f5, #89c3ff);
                            --primary-hover-gradient: linear-gradient(45deg, #4a91e0, #7ab2f0);
                            --background-color: #f0f2f5;
                            --card-background-color: #ffffff;
                            --text-color: #333;
                            --label-color: #555;
                            --border-color: #dde2e7;
                            --input-background: #f8f9fa;
                        }}
                        body {{
                            margin: 0;
                            font-family: 'Noto Sans KR', 'Inter', sans-serif;
                            background-color: var(--background-color);
                            color: var(--text-color);
                            display: flex;
                            justify-content: center;
                            align-items: flex-start; /* 이 부분을 수정했습니다. */
                            min-height: 100vh;
                            padding: 40px 20px; /* 이 부분을 수정했습니다. */
                            box-sizing: border-box;
                            overflow: auto; /* 이 부분을 수정했습니다. */
                        }}
                        .background-emojis span {{
                            position: absolute;
                            color: var(--primary-color);
                            font-size: 2rem;
                            opacity: 0.15;
                            animation: float 15s infinite linear;
                            z-index: 0;
                        }}
                        .background-emojis span:nth-child(1) {{ top: 10%; left: 15%; animation-duration: 18s; }}
                        .background-emojis span:nth-child(2) {{ top: 20%; left: 80%; animation-duration: 14s; }}
                        .background-emojis span:nth-child(3) {{ top: 70%; left: 10%; animation-duration: 20s; }}
                        .background-emojis span:nth-child(4) {{ top: 80%; left: 85%; animation-duration: 12s; }}
                        .background-emojis span:nth-child(5) {{ top: 40%; left: 5%; animation-duration: 22s; }}
                        .background-emojis span:nth-child(6) {{ top: 50%; left: 90%; animation-duration: 16s; }}
                        @keyframes float {{
                            0% {{ transform: translateY(0) rotate(0deg); }}
                            100% {{ transform: translateY(-120vh) rotate(360deg); }}
                        }}
                        .calculator-container {{
                            position: relative;
                            z-index: 1;
                            width: 100%;
                            max-width: 700px; /* 더 긴 내용을 위해 너비를 넓혔습니다. */
                            background-color: var(--card-background-color);
                            padding: 40px;
                            border-radius: 20px;
                            box-shadow: 0 10px 40px rgba(0, 0, 0, 0.1);
                            text-align: center;
                            transition: all 0.3s ease;
                            border: 1px solid rgba(255, 255, 255, 0.5);
                        }}
                        h1 {{
                            color: var(--primary-color);
                            margin-bottom: 20px;
                            font-size: 1.8rem;
                            font-weight: 700;
                        }}
                        .summary-box {{
                            border: 2px solid var(--primary-color);
                            border-radius: 15px;
                            padding: 20px;
                            margin: 20px auto;
                            max-width: 450px;
                            text-align: left;
                        }}
                        .summary-box p {{
                            margin: 10px 0;
                            font-size: 1.1rem;
                            font-weight: 500;
                        }}
                        .summary-box strong {{
                            color: var(--primary-color);
                            font-weight: 700;
                        }}
                        h2 {{
                            font-size: 1.5rem;
                            color: var(--text-color);
                            margin-top: 40px;
                            margin-bottom: 20px;
                            font-weight: 700;
                        }}
                        table {{
                            width: 100%;
                            border-collapse: collapse;
                            margin-top: 20px;
                            border-radius: 10px;
                            overflow: hidden;
                            box-shadow: 0 5px 20px rgba(0, 0, 0, 0.05);
                        }}
                        th, td {{
                            border: 1px solid var(--border-color);
                            padding: 12px 8px;
                            text-align: center;
                            font-size: 0.95rem;
                        }}
                        th {{
                            background-color: #f8f9fa;
                            color: #666;
                            font-weight: 700;
                        }}
                        tbody tr:nth-child(even) {{
                            background-color: #fcfcfc;
                        }}
                        a {{
                            display: inline-block;
                            margin-top: 30px;
                            padding: 15px;
                            border: none;
                            border-radius: 10px;
                            background: var(--primary-gradient);
                            color: white;
                            font-size: 1.1rem;
                            font-weight: 600;
                            cursor: pointer;
                            transition: all 0.3s ease;
                            text-decoration: none;
                        }}
                        a:hover {{
                            background: var(--primary-hover-gradient);
                            box-shadow: 0 4px 15px rgba(86, 163, 245, 0.3);
                            transform: translateY(-3px);
                        }}
                    </style>
                </head>
                <body>
                    <div class="background-emojis">
                        <span>💰</span>
                        <span>✨</span>
                        <span>💵</span>
                        <span>📈</span>
                        <span>🪙</span>
                        <span>💎</span>
                    </div>
                    <div class="calculator-container">
                        <h1>💰 계산 결과</h1>
                        <p>목표를 달성하기 위해 필요한 정보입니다.</p>
                        <div class="summary-box">
                            <p>매달 내야 할 수익률: <strong>{json_data['monthly_rate'] * 100:.2f}%</strong></p>
                            <p>총 투자금: <strong>{json_data['total_investment']:.0f} 만 원</strong></p>
                            <p>최종 금액: <strong>{json_data['final_capital']:.0f} 만 원</strong></p>
                            <p>총 수익: <strong>{json_data['total_profit']:.0f} 만 원</strong></p>
                        </div>
                        <h2>월별 상세 내역</h2>
                        <table>
                            <thead>
                                <tr>
                                    <th>월별</th>
                                    <th>월초 자본금</th>
                                    <th>내야 할 수익</th>
                                </tr>
                            </thead>
                            <tbody>
                                {table_rows}
                            </tbody>
                        </table>
                        <a href="/">다시 계산하기 🚀</a>
                    </div>
                </body>
                </html>
                """
                self.send_response(200)
                self.send_header('Content-type', 'text/html; charset=utf-8')
                self.end_headers()
                self.wfile.write(response_html.encode('utf-8'))
            except subprocess.CalledProcessError as e:
                self.send_error(500, f"C 프로그램 실행 실패: {e}")
            except Exception as e:
                self.send_error(500, f"서버 내부 오류 발생: {e}")

if __name__ == '__main__':
    try:
        with socketserver.TCPServer(("", PORT), MyHandler) as httpd:
            print("웹 서버가 http://localhost:%s 에서 실행 중입니다." % PORT)
            httpd.serve_forever()
    except Exception as e:
        print(f"서버 시작 중 오류 발생: {e}", file=sys.stderr)