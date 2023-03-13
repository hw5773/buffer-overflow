header = "<html><head><meta charset=\"UTF-8\"><title>나는 해커다 - 시스템 해킹 이해 및 실습 성과 보고회</title><link rel=\"stylesheet\" type=\"text/css\" href=\"https://cdn.jsdelivr.net/gh/moonspam/NanumSquare@1.0/nanumsquare.css\"><link rel=\"stylesheet\" type=\"text/css\" href=\"http://www.openhium.com/hangul/nanumbarungothic.css\"><link rel=\"stylesheet\" href=\"bof.css\" media=\"(max-width:1000px)\"><link rel=\"stylesheet\" href=\"bof.css\" media=\"(min-width:1000px)\"></head><body><div id=\"page\"><header><div id=\"title\"><b>교수님의 컴퓨터를 털어라!</b></div><div id=\"subtitle\">나는 해커다 - 시스템 해킹 이해 및 실습 성과 보고회</div></header><div id=\"prolog\">"

prolog = "나는 유능한 KENTECH의 <b>해커 A모씨</b>이다.<img class=\"emoticon\" src=\"hacker.png\" /><br>최근 \'컴퓨터 구조와 시스템\' 수업<br>교수님의 개인 컴퓨터를<br><b>해킹하는데 성공</b>하였다.<img class=\"emoticon\" src=\"attitude.jpg\" width=150px/><br><br>교수님의 컴퓨터 원격 접속 서버에는<br><b>심각한 취약점</b>이 있었고,<br>이를 활용하면 원격 접속 시<br>계정과 비밀번호를 적절히 잘 입력하여<br>교수님의 본래 계정과 비밀번호를 몰라도<br>접속할 수 있었다.<br><br>나는 이미 교수님의 로그인 계정과 비밀번호도 알아내었지만,<br>내 능력을 뽐내면서<br>KENTECH에 나와 같은 유능한 능력자가 있는지 찾기 위해 문제를 만들었다.<br>지금 이 페이지를 읽고 있는 당신도<br>정당하지 않은 방법으로<br>교수님의 컴퓨터를 털어보어라.<br><br>어디 감히 해볼 수 있겠는가?<img class=\"emoticon\" src=\"kkk.png\" width=150px/><br><br><b>A모씨가 공개한 서버의 소스코드를 참조하여<br>정당한 로그인 방법 이외의 방법으로<br>로그인에 성공해 보시오.</b><br><br><hr>"

main = "<form action=\"/\" method=\"post\"><div id=\"form\"><label for=\"account\">ID:</label><input type=\"text\" name=\"account\" size=\"30\" /></div><div><label for=\"password\">PW:</label><input type=\"password\" name=\"password\" size=\"30\" /></div><div><button>제출</button></div></form><hr><div id=\"hint\"><div id=\"valid\"><h3>정당한 로그인 방법</h3>아래는 정당한 ID와 패스워드입니다.<br><div class=\"box\">ID: lee<br>PW: correctpassword<br></div></div><br><div id=\"code\"><h3>서버 소스코드 일부</h3>아래는 A모씨가 공개한 취약점이 있는 서버 소스코드입니다. 아래 코드를 보면 인증 절차를 우회할 수 있는 방법이 있다고 하네요. 아래 변수 buf는 아래 코드와 동일한 함수에 선언되어 있는 배열이며, 여기에 여러분이 입력한 패스워드가 저장됩니다. <div class=\"box\">if (!strncmp((const char *)buf, answer, strlen(answer))) {<br>&nbsp;&nbsp;&nbsp;&nbsp;printf(\"[*] Password is correct!\");<br>&nbsp;&nbsp;&nbsp;&nbsp;pass = 1;<br>} else {<br>&nbsp;&nbsp;&nbsp;&nbsp;printf(\"[*] Password is incorrect!\");<br>}<br><br>if (pass) {<br>&nbsp;&nbsp;&nbsp;&nbsp;printf(\"[*] The client is successfully signed in and has the root privilege!\");<br>}</div></div>"

footer = "</div></div></div></body></html>"

def get_address(aname):
    ret = {}

    with open(aname, "r") as f:
        for line in f:
            name, addr = line.strip().split(": ")
            ret[name.strip()] = addr.strip()

    print ("ret: {}".format(ret))
    return ret

def make_address_div(addr):
    ret = "<div id=\"address\"><h3>서버에서 사용하는 변수들 주소</h3><div class=\"box\"><div>buf: {}</div><div>len: {}</div><div>ret: {}</div><div>pass: {}</div><div>answer: {}</div></div></div>".format(addr["buf"], addr["len"], addr["ret"], addr["pass"], addr["answer"])
    return ret

def make_index_html(addr):
    ret = header
    ret += prolog
    ret += main
    ret += make_address_div(addr)
    ret += footer
    return ret

def make_success_html(addr):
    ret = header
    ret += "<img src=\"success.png\" />"
    ret += main
    ret += make_address_div(addr)
    ret += footer
    return ret

def make_failure_html(addr):
    ret = header
    ret += "<img src=\"failure.png\" />"
    ret += main
    ret += make_address_div(addr)
    ret += footer
    return ret

def make_valid_html(addr):
    ret = header
    ret += "정당한 계정으로 로그인 성공했군요.<br>그러나 우리는 정당하지 않은 방법으로 해야합니다!"
    ret += main
    ret += make_address_div(addr)
    ret += footer
    return ret
