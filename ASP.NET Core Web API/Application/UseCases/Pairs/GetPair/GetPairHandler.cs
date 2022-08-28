using System.Threading;
using System.Threading.Tasks;
using Domain.RepositoryInterfaces;
using MediatR;

namespace Application.UseCases.Pairs.GetPair;

public class GetPairHandler : IRequestHandler<GetPairRequest, GetPairResponse>
{
    private readonly IPairRepository _pairRepository;

    public GetPairHandler(IPairRepository pairRepository)
    {
        _pairRepository = pairRepository;
    }
        
    public async Task<GetPairResponse> Handle(GetPairRequest request, CancellationToken cancellationToken)
    {
        request.Validate();

        var pair = await _pairRepository.GetAsync(request.Id, cancellationToken);

        return new GetPairResponse(pair);
    }
}